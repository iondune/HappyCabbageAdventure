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

void CSceneObject::setMaterial(CMaterial const & material)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->getMaterial() = material;
}

void CSceneObject::setTexture(CTexture * texture)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->getMaterial().Texture = texture;
}

void CSceneObject::setTexture(std::string const & texture)
{
	setTexture(CImageLoader::loadTexture(texture));
}

void CSceneObject::setTexture(CTexture * texture, int const renderable)
{
	//Renderables[renderable].Texture = texture;
	std::vector<CRenderable *>::iterator it = Renderables.begin();
	for (int i = 0; i < renderable && it != Renderables.end(); ++ i, ++ it)
		;
	(* it)->getMaterial().Texture = texture;
}

void CSceneObject::setTexture(std::string const & texture, int const renderable)
{
	setTexture(CImageLoader::loadTexture(texture), renderable);
}

void CSceneObject::addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->addAttribute(label, attribute);
}

void CSceneObject::addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->addUniform(label, uniform);
}

void CSceneObject::removeAttribute(std::string const & label)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->removeAttribute(label);
}

void CSceneObject::removeUniform(std::string const & label)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->removeUniform(label);
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
