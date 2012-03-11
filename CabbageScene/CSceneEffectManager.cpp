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
	: DoSSAO(false), OnlySSAO(false), DoBloom(true), DoBlur(false), OnlyNormals(false)
{}

void CSceneEffectManager::apply()
{}
