#include "CSceneObject.h"

#include "CShaderLoader.h"
#include "CImageLoader.h"


CSceneObject::CSceneObject()
{}

void CSceneObject::addSceneObject(CRenderable * Renderable)
{
	Renderables.push_back(Renderable);
}

void CSceneObject::removeSceneObject(CRenderable * Renderable)
{
	Renderables.erase(std::remove(Renderables.begin(), Renderables.end(), Renderable), Renderables.end());
}

void CSceneObject::draw(CScene const * const scene)
{
   if(!Visible)
      return;
	ISceneObject::draw(scene);

	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->draw(scene);
}

void CSceneObject::setShader(CShader * shader)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setShader(shader);
}

void CSceneObject::setShader(std::string const & shader)
{
	setShader(CShaderLoader::loadShader(shader));
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
