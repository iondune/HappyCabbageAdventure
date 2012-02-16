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
