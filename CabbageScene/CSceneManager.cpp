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
	EFBO_SSAO_RAW,
	EFBO_SSAO_BLUR1,
	EFBO_SSAO_BLUR2,
	EFBO_SSAO_NORMALS,
	FBO_COUNT 
};

GLuint textureId[FBO_COUNT];
GLuint fboId[FBO_COUNT];
GLuint rboId[FBO_COUNT];
CShader * SSAOShader;
CShader * BlendShader, * BlurV, * BlurH;
CTexture * White, *Black, *Magenta;
SPosition2 ScreenSize;
GLuint randNorm;

#include "CTextureLoader.h"
#define SSAO_MULT 1

CSceneManager::CSceneManager(SPosition2 const & screenSize)
	: DoSSAO(false), OnlySSAO(false), DoBloom(true), DoBlur(false), OnlyNormals(false), FinalBlurSize(0.0f)
{
    CurrentScene = this;

	/*
	 * Create a simple quad VBO to use for draw operations!
	 */
	GLfloat QuadVertices[] = 
	{
		-1.0, -1.0,
		 1.0, -1.0,
		 1.0,  1.0,
		-1.0,  1.0
	};

	glGenBuffers(1, & QuadHandle);
	glBindBuffer(GL_ARRAY_BUFFER, QuadHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

	ScreenSize = screenSize;

	// create fbos!
	for (int i = 0; i < FBO_COUNT; ++ i)
	{
		unsigned int  TEXTURE_WIDTH = screenSize.X / (i == EFBO_SSAO_RAW? SSAO_MULT : 1);
		unsigned int  TEXTURE_HEIGHT = screenSize.Y / (i == EFBO_SSAO_RAW? SSAO_MULT : 1);

		glGenTextures(1, & textureId[i]);
		glBindTexture(GL_TEXTURE_2D, textureId[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// For some reason this is needed to make it work
		bool const Work = i == EFBO_SSAO_NORMALS;

		if (Work)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
		{
			//fboUsed = false;
			std::cerr << "Failed to make FBO!!!!!! -----------------" << std::endl  << std::endl  << std::endl;
		}

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	SSAOShader = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "SSAO.frag");
	BlendShader = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "Blend.frag");
	BlurV = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "BlurV.frag");
	BlurH = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "BlurH.frag");
	White = CTextureLoader::loadTexture("Colors/White.bmp");
	Black = CTextureLoader::loadTexture("Colors/Black.bmp");
	Magenta = CTextureLoader::loadTexture("Colors/Magenta.bmp");

	randNorm = CTextureLoader::loadTexture("SSAO/RandNormals.bmp")->getTextureHandle();
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
	if (DoSSAO || OnlyNormals)
	{
		// Draw normal colors
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[OnlyNormals ? EFBO_SCENE : EFBO_SSAO_NORMALS]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RootObject.drawNormals(CurrentScene);
	}
#endif

	// Draw regular scene
	if (! OnlyNormals)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCENE]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RootObject.draw(CurrentScene);
	}


	// Setup for quad rendering
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);


#ifdef SSAO
	if (DoSSAO)
	{
		// Draw SSAO effect
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_RAW]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			CShaderContext Context(* SSAOShader);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SSAO_NORMALS]);
			Context.uniform("normalMap", 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, randNorm);
			Context.uniform("rnm", 1);

			glViewport(0, 0, ScreenSize.X / SSAO_MULT, ScreenSize.Y / SSAO_MULT);

			Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);

			::glViewport(0, 0, ScreenSize.X, ScreenSize.Y);
		}

		if (DoBlur)
		{
			// Draw blur 1
			glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_BLUR1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			{
				CShaderContext Context(* BlurV);

				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SSAO_RAW]);

				//glGenerateMipmap(GL_TEXTURE_2D);

				Context.uniform("uTexColor", 0);

				Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);

			}

			// Draw blur 2
			glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_BLUR2]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			{
				CShaderContext Context(* BlurH);

				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SSAO_BLUR1]);

            Context.uniform("BlurSize", 1.0f);
            Context.uniform("DimAmount", 1.0f);

				//glGenerateMipmap(GL_TEXTURE_2D);

				Context.uniform("uTexColor", 0);

				Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
			}
		}
	}
#endif

   if (DoBloom)
   {
      //BLURH
      // Draw blurH effect
      glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw blurV quad
      {
         glEnable(GL_TEXTURE_2D);
         CShaderContext Context(* BlurH);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCENE]);
         //glGenerateMipmap(GL_TEXTURE_2D);
         Context.uniform("BlurSize", 1.0f);
         Context.uniform("DimAmount", 1.0f);

         Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
      }

      //BLURV
      // Draw blurV effect
      glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH2]);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw blurV quad
      {
		  CShaderContext Context(* BlurV);
         glEnable(GL_TEXTURE_2D);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCRATCH1]);
         //glGenerateMipmap(GL_TEXTURE_2D);

		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
      }
   }

	glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
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
		glBindTexture(GL_TEXTURE_2D, DoSSAO ? textureId[DoBlur ? EFBO_SSAO_BLUR2 : EFBO_SSAO_RAW] : White->getTextureHandle());
		//glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, DoBloom ? textureId[EFBO_SCRATCH2] : Magenta->getTextureHandle());
		//glGenerateMipmap(GL_TEXTURE_2D);
		
		Context.uniform("scene", 0);
		Context.uniform("ssao", 1);
		Context.uniform("bloom", 2);

		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

   SceneChanged = false;
}

#include "CApplication.h"
void CSceneManager::blurSceneIn(float seconds, float const RunTime)
{
	std::cout << "Blurring in scene..." << std::endl;
	BlurOutTime = 0;
	BlurInTime = seconds;
	CurTime = RunTime;
}

void CSceneManager::blurSceneOut(float seconds, float const RunTime)
{
	BlurOutTime = seconds;
	BlurInTime = 0;
	CurTime = RunTime;


	float now = RunTime;
	float difference = RunTime - now;

	float drawTimer = 10.0f;

	while (difference < seconds)
	{
		CApplication::get().updateTime();

		Dim = (seconds - difference)/seconds;
		FinalBlurSize = difference/ 0.1f;

		if (drawTimer == 10.0f)
		{
			endDraw();
			SDL_GL_SwapBuffers();
		}

		drawTimer -= CApplication::get().getRunTime() - difference;

		if (drawTimer <= 0.0f)
			drawTimer = 10.0f;

		difference = CApplication::get().getRunTime() - now;
	}
}

void CSceneManager::endDraw()
{
	if(CurTime == -1.0f)
		Dim = 1.0f;

	if(BlurInTime > 0.0f)
	{
		FinalBlurSize = 0.0f;
		Dim = std::max(1.0f - (BlurInTime - (CApplication::get().getRunTime() - CurTime))/BlurInTime, 0.0f);
		if((CApplication::get().getRunTime() - CurTime) > BlurInTime)
		{
			BlurInTime = 0.0f;
			CurTime = -1.0f;
		}
	}

	// Setup for quad rendering
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw Texture
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //glUseProgram(0); //Doesn't help

   // THE FINAL RENDER
	// Draw SSAO quad
	{
		glEnable(GL_TEXTURE_2D);
      CShaderContext Context(* BlurH);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId[EFBO_SCRATCH1]);
      Context.uniform("BlurSize", FinalBlurSize);
      Context.uniform("DimAmount", Dim);
		
		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
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


void CSceneManager::load()
{
	RootObject.load(this);
}
