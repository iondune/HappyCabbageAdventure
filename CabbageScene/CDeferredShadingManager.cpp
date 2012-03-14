#include "CDeferredShadingManager.h"

#include "CSceneManager.h"
#include "CShaderLoader.h"

CDeferredShadingManager::CDeferredShadingManager(CSceneManager * sceneManager)
	: CSceneEffectManager(sceneManager)
{

	DeferredOutputTarget = new CFrameBufferObject();
	DeferredColorOutput = new CTexture(SceneManager->getScreenSize(), false);
	STextureCreationFlags Flags;
	Flags.MipMaps = false;
	//Flags.PixelFormat = GL_RED;
	Flags.PixelInternalFormat = GL_RGB32F;
	Flags.PixelType = GL_FLOAT;
	DeferredPositionOutput = new CTexture(SceneManager->getScreenSize(), false, Flags);
	DeferredNormalOutput = new CTexture(SceneManager->getScreenSize(), false);

	DeferredOutputTarget->attach(DeferredColorOutput, GL_COLOR_ATTACHMENT0);
	DeferredOutputTarget->attach(DeferredPositionOutput, GL_COLOR_ATTACHMENT1);
	DeferredOutputTarget->attach(DeferredNormalOutput, GL_COLOR_ATTACHMENT2);
	DeferredOutputTarget->attach(new CRenderBufferObject(GL_DEPTH_COMPONENT, SceneManager->getScreenSize()), GL_DEPTH_ATTACHMENT);

	LightPassOutputTarget = new CFrameBufferObject();
	LightPassColorOutput = new CTexture(SceneManager->getScreenSize(), false);
	LightPassOutputTarget->attach(LightPassColorOutput, GL_COLOR_ATTACHMENT0);


	DeferredOutputTarget->bind();
	GLenum Buffers[] = 
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};

	glDrawBuffers(sizeof(Buffers)/sizeof(GLenum), Buffers);


	RenderPasses.clear();

	SRenderPass DeferredPass;
	DeferredPass.Pass = ERP_DEFERRED_OBJECTS;
	DeferredPass.Target = DeferredOutputTarget;

	RenderPasses.push_back(DeferredPass);


	SRenderPass LightPass;
	LightPass.Pass = ERP_DEFERRED_LIGHTS;
	LightPass.Target = LightPassOutputTarget;

	RenderPasses.push_back(LightPass);


	FinalBlendShader = CShaderLoader::loadShader("Deferred/Blend");
}

void CDeferredShadingManager::apply()
{
#if 1
	SPostProcessPass BlendPass;
	BlendPass.Textures["uTexColor"] = LightPassColorOutput;
	//BlendPass.Textures["uLightPass"] = LightPassColorOutput;
	BlendPass.Target = SceneManager->getSceneFrameBuffer();
	BlendPass.Shader = CShaderLoader::loadShader("FBO/QuadCopy");//FinalBlendShader;
#else
	SPostProcessPass BlendPass;
	BlendPass.Textures["uSceneColor"] = DeferredColorOutput;
	BlendPass.Textures["uLightPass"] = LightPassColorOutput;
	BlendPass.Target = SceneManager->getSceneFrameBuffer();
	BlendPass.Shader = FinalBlendShader;
#endif
	BlendPass.doPass();
}

void CDeferredShadingManager::setEffectEnabled(ESceneEffect const Effect, bool const Enabled)
{
	return;
}

bool const CDeferredShadingManager::isEffectEnabled(ESceneEffect const Effect)
{
	return false;
}
