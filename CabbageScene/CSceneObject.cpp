#include "CSceneObject.h"

#include "CShaderLoader.h"
#include "CImageLoader.h"


CSceneObject::CSceneObject()
{}

void CSceneObject::setShader(ERenderPass const Pass, CShader * shader)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setShader(Pass, shader);
}

void CSceneObject::setShader(ERenderPass const Pass, std::string const & shader)
{
	setShader(Pass, CShaderLoader::loadShader(shader));
}

CShader const * const CSceneObject::getShader() const
{
	CShader const * Shader = 0;
	for (std::vector<CRenderable *>::const_iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(ERP_DEFAULT);
		else if (Shader != (* it)->getShader(ERP_DEFAULT))
			return 0;
	}
	return Shader;
}

CShader * CSceneObject::getShader()
{
	CShader * Shader = 0;
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(ERP_DEFAULT);
		else if (Shader != (* it)->getShader(ERP_DEFAULT))
			return 0;
	}
	return Shader;
}

CShader const * const CSceneObject::getShader(ERenderPass const Pass) const
{
	CShader const * Shader = 0;
	for (std::vector<CRenderable *>::const_iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(Pass);
		else if (Shader != (* it)->getShader(Pass))
			return 0;
	}
	return Shader;
}

CShader * CSceneObject::getShader(ERenderPass const Pass)
{
	CShader * Shader = 0;
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(Pass);
		else if (Shader != (* it)->getShader(Pass))
			return 0;
	}
	return Shader;
}
