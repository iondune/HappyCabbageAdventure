#include "CSceneEffectManager.h"

#include "CSceneManager.h"


CSceneEffectManager::SPostProcessPass::SPostProcessPass()
	: Target(0), Shader(0)
{}

void CSceneEffectManager::SPostProcessPass::doPass()
{
	if (Target)
		Target->bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CShaderContext Context(* Shader);

	for (std::map<std::string, CTexture *>::iterator it = Textures.begin(); it != Textures.end(); ++ it)
		Context.bindTexture(it->first, it->second);

	for (std::map<std::string, float>::iterator it = Floats.begin(); it != Floats.end(); ++ it)
		Context.uniform(it->first, it->second);

	for (std::map<std::string, int>::iterator it = Ints.begin(); it != Ints.end(); ++ it)
		Context.uniform(it->first, it->second);

	Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

	glDrawArrays(GL_QUADS, 0, 4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

CSceneEffectManager::CSceneEffectManager()
	: EnabledEffects(0)
{}

void CSceneEffectManager::apply()
{
	/*glDisable(GL_DEPTH_TEST);

	if (DoSSAO)
	{
		// Draw SSAO effect
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_RAW]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			CShaderContext Context(* SSAOShader);
			
			Context.bindTexture("normalMap", textureId[EFBO_SSAO_NORMALS]);
			Context.bindTexture("rnm", randNorm);
			Context.bindBufferObject("aPosition", QuadHandle, 2);

			glDrawArrays(GL_QUADS, 0, 4);
		}

		if (DoBlur)
		{
			// Draw blur 1
			glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_BLUR1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			{
				CShaderContext Context(* BlurV);

				Context.bindTexture("uTexColor", textureId[EFBO_SSAO_RAW]);
				Context.bindBufferObject("aPosition", QuadHandle, 2);

				glDrawArrays(GL_QUADS, 0, 4);

			}

			// Draw blur 2
			glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SSAO_RAW]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			{
				CShaderContext Context(* BlurH);

				Context.uniform("BlurSize", 1.0f);
				Context.uniform("DimAmount", 1.0f);
				Context.bindTexture("uTexColor", textureId[EFBO_SSAO_BLUR1]);
				Context.bindBufferObject("aPosition", QuadHandle, 2);

				glDrawArrays(GL_QUADS, 0, 4);
			}
		}
	}

	if (DoBloom)
	{
		//BLURH
		// Draw blurH effect
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw blurV quad
		{
			CShaderContext Context(* BlurH);

			Context.bindTexture("uTexColor", SceneFrameTexture);
			Context.uniform("BlurSize", 1.0f);
			Context.uniform("DimAmount", 1.0f);
			Context.bindBufferObject("aPosition", QuadHandle, 2);

			glDrawArrays(GL_QUADS, 0, 4);
		}

		//BLURV
		// Draw blurV effect
		glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH2]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			CShaderContext Context(* BlurV);

			Context.bindTexture("uTexColor", textureId[EFBO_SCRATCH1]);
			Context.bindBufferObject("aPosition", QuadHandle, 2);

			glDrawArrays(GL_QUADS, 0, 4);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fboId[EFBO_SCRATCH1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Final Render
	{
		CShaderContext Context(* BlendShader);
		
		if (OnlySSAO)
			Context.bindTexture("scene", White);
		else
			Context.bindTexture("scene", SceneFrameTexture);
		Context.bindTexture("ssao", DoSSAO ? textureId[EFBO_SSAO_RAW] : White->getTextureHandle());
		Context.bindTexture("bloom", DoBloom ? textureId[EFBO_SCRATCH2] : Magenta->getTextureHandle());
		Context.bindBufferObject("aPosition", QuadHandle, 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}*/
}

void CSceneEffectManager::setEffectEnabled(ESceneEffect const Effect, bool const Enabled)
{
	if (Enabled)
		EnabledEffects |= Effect;
	else
		EnabledEffects ^= Effect;

	switch (Effect)
	{
	case ESE_SSAO:
		if (Enabled)
		{

		}
		else
		{

		}
		break;

	case ESE_SSAO_BLUR:
		break;

	case ESE_BLOOM:
		break;

	}
}

bool const CSceneEffectManager::isEffectEnabled(ESceneEffect const Effect)
{
	return EnabledEffects & Effect;
}
