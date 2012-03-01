#include "CSceneManager.h"

#include <algorithm>
#include <sstream>

#include "CShaderLoader.h"
#include "CMeshLoader.h"


CLight const CScene::NullLight;


CScene::CScene()
   : BindProjMatrix(ProjMatrix), BindViewMatrix(ViewMatrix), BindLightCount(LightCount), UseCulling(true)
{
    ActiveCamera = & DefaultCamera;

    addUniform("uProjMatrix", & BindProjMatrix);
    addUniform("uViewMatrix", & BindViewMatrix);
    addUniform("uLightCount", & BindLightCount);
}

void CScene::addUniform(std::string const & label, IUniform const * uniform)
{
    Uniforms[label] = uniform;
}

void CScene::removeUniform(std::string const & label)
{
    std::map<std::string, IUniform const *>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        Uniforms.erase(it);
}

ICamera * const CScene::getActiveCamera()
{
    return ActiveCamera;
}

ICamera const * const CScene::getActiveCamera() const
{
    return ActiveCamera;
}

void CScene::setActiveCamera(ICamera * const activeCamera)
{
    ActiveCamera = activeCamera;
}

bool const CScene::isCullingEnabled() const
{
   return UseCulling;
}

void CScene::setCullingEnabled(bool const culling)
{
   UseCulling = culling;
}


unsigned int const digitCount(int n)
{
    int count = 1;
    if (n < 0)
    {
        n *= -1;
        ++ count;
    }

    while (n > 10)
    {
        ++ count;
        n /= 10;
    }

    return count;
}

IUniform const * CScene::getUniform(std::string const & label) const
{
    if (label.substr(0, 8) == "uLights[")
    {
        std::stringstream ss(label.substr(8));
        unsigned int index;
        ss >> index;
        std::string remaining = ss.str();
        remaining = remaining.substr(2 + digitCount(index));

        if (remaining == "Color")
        {
            if (index >= Lights.size())
            return & NullLight.BindColor;
            else
                return & Lights[index]->BindColor;
        }
        else if (remaining == "Position")
        {
            if (index >= Lights.size())
                return & NullLight.BindPosition;
            else
                return & Lights[index]->BindPosition;
        }
    }

    std::map<std::string, IUniform const *>::const_iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        return it->second;

    return 0;
}

void CScene::update()
{
	ActiveCamera->recalculateViewMatrix();
	ViewMatrix = ActiveCamera->getViewMatrix();
	ProjMatrix = ActiveCamera->getProjectionMatrix();

	if (LightCount != Lights.size())
	{
		SceneChanged = true;
		LightCount = Lights.size();
	}

	RootObject.updateAbsoluteTransformation();
	RootObject.update();
}

enum EFBOID
{
	EFBO_SCENE,
	EFBO_SCRATCH1,
	EFBO_SCRATCH2,
	EFBO_SSAO,
	FBO_COUNT 
};

GLuint textureId[FBO_COUNT];
GLuint fboId[FBO_COUNT];
GLuint rboId[FBO_COUNT];
CShader * SSAOShader;
CShader * BlendShader;
CTexture * White;

#include "CTextureLoader.h"

CSceneManager::CSceneManager(SPosition2 const & screenSize)
	: DoSSAO(false), OnlySSAO(false)
{
    CurrentScene = this;

	bool fboUsed = true;
	

	// create a texture object
	for (int i = 0; i < FBO_COUNT; ++ i)
	{
		unsigned int  TEXTURE_WIDTH = screenSize.X;
		unsigned int  TEXTURE_HEIGHT = screenSize.Y;

		glGenTextures(1, &textureId[i]);
		glBindTexture(GL_TEXTURE_2D, textureId[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// create a renderbuffer object to store depth info
		glGenRenderbuffers(1, & rboId[i]);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
								 TEXTURE_WIDTH, TEXTURE_HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// create a framebuffer object
		glGenFramebuffers(1, & fboId[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[i]);

		// attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								  GL_TEXTURE_2D, textureId[i], 0);

		// attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
									 GL_RENDERBUFFER, rboId[i]);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			fboUsed = false;

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	SSAOShader = CShaderLoader::loadShader("SSAO");
	BlendShader = CShaderLoader::loadShader("Blend");
	White = CTextureLoader::loadTexture("White.bmp");
}

void CSceneManager::addSceneObject(ISceneObject * sceneObject)
{
   RootObject.addChild(sceneObject);
}

void CSceneManager::removeSceneObject(ISceneObject * sceneObject)
{
   RootObject.removeChild(sceneObject);
}

void CSceneManager::removeAllSceneObjects()
{
   RootObject.removeChildren();
}

#define SSAO

void CSceneManager::drawAll()
{
    CurrentScene->update();

#ifdef SSAO
	if (DoSSAO)
	{
	// Draw normal colors
	glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	enableDebugData(EDebugData::NormalColors);
    RootObject.draw(CurrentScene);
	disableDebugData(EDebugData::NormalColors);
	}
#endif

	// Draw regular scene
	glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCENE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RootObject.draw(CurrentScene);


#ifdef SSAO
	if (DoSSAO)
	{
	// Draw SSAO effect
	glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw SSAO quad
	{
		CShaderContext Context(* SSAOShader);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCRATCH1]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, CTextureLoader::loadTexture("randNormals.bmp")->getTextureHandle());
		glGenerateMipmap(GL_TEXTURE_2D);

		Context.uniform("rnm", 1);
		Context.uniform("normalMap", 0);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_DEPTH_TEST);

			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);

				glTexCoord2i(1, 0);
				glVertex2i(1, 0);

				glTexCoord2i(1, 1);
				glVertex2i(1, 1);
			
				glTexCoord2i(0, 1);
				glVertex2i(0, 1);
			glEnd();

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	}
#endif

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Draw Texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SSAO]);
	glGenerateMipmap(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);
			glVertex2i(0, 0);

			glTexCoord2i(1, 0);
			glVertex2i(1, 0);

			glTexCoord2i(1, 1);
			glVertex2i(1, 1);
			
			glTexCoord2i(0, 1);
			glVertex2i(0, 1);
		glEnd();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);*/

   if (DoBloom)
   {
      //BLURH
      // Draw blurH effect
      glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram((CShaderLoader::loadShader("BlurH"))->getProgramHandle());

      // Draw blurV quad
      {
         glEnable(GL_TEXTURE_2D);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCENE]);
         glGenerateMipmap(GL_TEXTURE_2D);

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glOrtho(0, 1, 0, 1, -1, 1);

         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         glDisable(GL_DEPTH_TEST);

         glBegin(GL_QUADS);
         glTexCoord2i(0, 0);
         glVertex2i(0, 0);

         glTexCoord2i(1, 0);
         glVertex2i(1, 0);

         glTexCoord2i(1, 1);
         glVertex2i(1, 1);

         glTexCoord2i(0, 1);
         glVertex2i(0, 1);
         glEnd();

         glEnable(GL_DEPTH_TEST);
         glDisable(GL_TEXTURE_2D);

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, 0);
      }

      //BLURV
      // Draw blurV effect
      glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH2]);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram((CShaderLoader::loadShader("BlurV"))->getProgramHandle());

      // Draw blurV quad
      {
         glEnable(GL_TEXTURE_2D);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCRATCH1]);
         glGenerateMipmap(GL_TEXTURE_2D);

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glOrtho(0, 1, 0, 1, -1, 1);

         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         glDisable(GL_DEPTH_TEST);

         glBegin(GL_QUADS);
         glTexCoord2i(0, 0);
         glVertex2i(0, 0);

         glTexCoord2i(1, 0);
         glVertex2i(1, 0);

         glTexCoord2i(1, 1);
         glVertex2i(1, 1);

         glTexCoord2i(0, 1);
         glVertex2i(0, 1);
         glEnd();

         glEnable(GL_DEPTH_TEST);
         glDisable(GL_TEXTURE_2D);

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, 0);
         glUseProgram(0);
      }
   }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw Texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



   // THE FINAL RENDER
	// Draw SSAO quad
	{
		CShaderContext Context(* BlendShader);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, OnlySSAO ? White->getTextureHandle() : textureId[EFBO_SCENE]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, DoSSAO ? textureId[EFBO_SSAO] : White->getTextureHandle());
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, DoBloom ? textureId[EFBO_SCRATCH2] : White->getTextureHandle());
		glGenerateMipmap(GL_TEXTURE_2D);
		
		Context.uniform("scene", 0);
		Context.uniform("ssao", 1);
		Context.uniform("bloom", 2);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_DEPTH_TEST);

			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);

				glTexCoord2i(1, 0);
				glVertex2i(1, 0);

				glTexCoord2i(1, 1);
				glVertex2i(1, 1);
			
				glTexCoord2i(0, 1);
				glVertex2i(0, 1);
			glEnd();

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    SceneChanged = false;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(Shader);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(Shader);
   Object->setMaterial(Material);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(std::string const & Mesh, std::string const & Shader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(CMeshLoader::load3dsMesh(Mesh));
   Object->setShader(CShaderLoader::loadShader(Shader));
   Object->setMaterial(Material);
   addSceneObject(Object);
   return Object;
}


	
void CScene::enableDebugData(EDebugData::Domain const type)
{
	RootObject.enableDebugData(type);
}

void CScene::disableDebugData(EDebugData::Domain const type)
{
	RootObject.disableDebugData(type);
}
