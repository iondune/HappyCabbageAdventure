#include "CSceneObject.h"

CSceneObject::CSceneObject()
{}

void CSceneObject::addRenderable(CRenderable * Renderable)
{
	Renderables.push_back(Renderable);
}

void CSceneObject::removeRenderable(CRenderable * Renderable)
{
	Renderables.erase(std::remove(Renderables.begin(), Renderables.end(), Renderable), Renderables.end());
}

void CSceneObject::draw(CScene const * const scene)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->draw(scene);
}

void CSceneObject::setShader(CShader * shader)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setShader(shader);
}

void CSceneObject::setMaterial(CMaterial const & material)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->getMaterial() = material;
}
