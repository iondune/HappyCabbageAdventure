#include "CSceneManager.h"

#include <algorithm>
#include <sstream>

#include "CShaderLoader.h"
#include "CMeshLoader.h"
#include "CTextureLoader.h"

#include "CDeferredShadingManager.h"


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


GLuint CSceneManager::QuadHandle = 0;

CSceneManager::CSceneManager(SPosition2 const & screenSize)
	: FinalBlurSize(0.0f), SceneFrameBuffer(0),
	EffectManager(0), ScreenSize(screenSize)
{
    CurrentScene = this;

	// Create a simple quad VBO to use for draw operations!
	if (! QuadHandle)
	{
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
	}


	STextureCreationFlags Flags;
	Flags.MipMaps = false;
	SceneFrameTexture = new CTexture(ScreenSize, true, Flags);
	SceneDepthBuffer = new CRenderBufferObject(GL_DEPTH_COMPONENT, ScreenSize);

	SceneFrameBuffer = new CFrameBufferObject();
	SceneFrameBuffer->attach(SceneDepthBuffer, GL_DEPTH_ATTACHMENT);
	SceneFrameBuffer->attach(SceneFrameTexture, GL_COLOR_ATTACHMENT0);	

	if (! SceneFrameBuffer->isValid())
		std::cerr << "Failed to make FBO for scene drawing!!!!!!" << std::endl  << std::endl  << std::endl;
	
	EffectManager = new CDeferredShadingManager(this);
	//EffectManager->setEffectEnabled(ESE_BLOOM, true);

	BlurHorizontal = CShaderLoader::loadShader("FBO/QuadCopyUV.glsl", "BlurH.frag");
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

void CSceneManager::drawAll()
{
    CurrentScene->update();

	if (EffectManager)
	{
		for (std::vector<CSceneEffectManager::SRenderPass>::iterator it = EffectManager->RenderPasses.begin(); it != EffectManager->RenderPasses.end(); ++ it)
		{
			if (it->Target)
				it->Target->bind();
			else
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

			if (it->Pass == ERP_DEFERRED_LIGHTS)
			{
				glClearColor(0.f,0.f,0.f,0.f);
				glDisable(GL_DEPTH_TEST);

				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RootObject.draw(CurrentScene, it->Pass);

			if (it->Pass == ERP_DEFERRED_LIGHTS)
			{
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			}
		}

		EffectManager->apply();
	}
	else
	{
		SceneFrameBuffer->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RootObject.draw(CurrentScene, ERP_DEFAULT);
	}
	
	SceneChanged = false;

	SceneFrameBuffer->bind();
}

void CSceneManager::blurSceneIn(float seconds, float const RunTime)
{
	std::cout << "Blurring in scene..." << std::endl;
	BlurOutTime = 0;
	BlurInTime = seconds;
	CurTime = RunTime;
}

#include <CApplication.h>
void CSceneManager::endDraw()
{
	// Do fade-in
	if (CurTime == -1.0f)
		Dim = 1.0f;

	if (BlurInTime > 0.0f)
	{
		FinalBlurSize = 0.0f;
		Dim = std::max(1.0f - (BlurInTime - (CApplication::get().getRunTime() - CurTime))/BlurInTime, 0.0f);
		if ((CApplication::get().getRunTime() - CurTime) > BlurInTime)
		{
			BlurInTime = 0.0f;
			CurTime = -1.0f;
		}
	}

	
	// Draw to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// THE FINAL RENDER
	{
		CShaderContext Context(* BlurHorizontal);

		Context.bindTexture("uTexColor", SceneFrameTexture);
		Context.uniform("BlurSize", FinalBlurSize);
		Context.uniform("DimAmount", Dim);
		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	glEnable(GL_DEPTH_TEST);
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(ERP_DEFAULT, Shader);
   Object->setShader(ERP_DEFERRED_OBJECTS, DeferredShader);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CShader * DeferredShader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(ERP_DEFAULT, Shader);
   Object->setShader(ERP_DEFERRED_OBJECTS, DeferredShader);
   Object->setMaterial(Material);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(std::string const & Mesh, std::string const & Shader, std::string const & DeferredShader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(CMeshLoader::load3dsMesh(Mesh));
   Object->setShader(ERP_DEFAULT, CShaderLoader::loadShader(Shader));
   Object->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader(DeferredShader));
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
	RootObject.load(this, ERP_DEFAULT);
}

CFrameBufferObject * CSceneManager::getSceneFrameBuffer()
{
	return SceneFrameBuffer;
}

CTexture * CSceneManager::getSceneFrameTexture()
{
	return SceneFrameTexture;
}

CRenderBufferObject * CSceneManager::getSceneDepthBuffer()
{
	return SceneDepthBuffer;
}

CSceneEffectManager * CSceneManager::getEffectManager()
{
	return EffectManager;
}

void CSceneManager::setEffectManager(CSceneEffectManager * effectManager)
{
	EffectManager = effectManager;
}

GLuint const CSceneManager::getQuadHandle()
{
	return QuadHandle;
}

SSize2 const & CSceneManager::getScreenSize() const
{
	return ScreenSize;
}
