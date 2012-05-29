#include "CSceneObject.h"

#include <CImageLoader.h>


void CSceneObject::setMaterial(CRenderable::SMaterial const & material)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setMaterial(material);
}

void CSceneObject::setMaterial(CRenderable::SMaterial const & material, unsigned int const Renderable)
{
	CRenderable * Child = getRenderable(Renderable);
	if (Child)
		Child->setMaterial(material);
}

void CSceneObject::setMaterial(CRenderable::SMaterial const & material, CRenderable * const Renderable)
{
	Renderable->setMaterial(material);
}


void CSceneObject::setTexture(unsigned int const Layer, CTexture * Texture)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setTexture(Layer, Texture);
}

void CSceneObject::setTexture(unsigned int const Layer, std::string const & texture)
{
	setTexture(Layer, CImageLoader::loadTexture(texture));
}

void CSceneObject::setTexture(unsigned int const Layer, CTexture * Texture, unsigned int const renderable)
{
	Renderables[renderable]->setTexture(Layer, Texture);
}

void CSceneObject::setTexture(unsigned int const Layer, std::string const & texture, unsigned int const renderable)
{
	setTexture(Layer, CImageLoader::loadTexture(texture), renderable);
}

void CSceneObject::setTexture(unsigned int const Layer, CTexture * Texture, CRenderable * const Renderable)
{
	Renderable->setTexture(Layer, Texture);
}

void CSceneObject::setTexture(unsigned int const Layer, std::string const & texture, CRenderable * const Renderable)
{
	setTexture(Layer, CImageLoader::loadTexture(texture), Renderable);
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
