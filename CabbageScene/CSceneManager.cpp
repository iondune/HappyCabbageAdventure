#include "CSceneManager.h"

#include <algorithm>


CSceneManager::CSceneManager()
    : ActiveCamera(0)
{
    ActiveCamera = new CCamera();
}

void CSceneManager::addRenderable(IRenderable * Renderable)
{
    Renderables.push_back(Renderable);
}

void CSceneManager::removeRenderable(IRenderable * Renderable)
{
    std::remove(Renderables.begin(), Renderables.end(), Renderable);
}

void CSceneManager::drawAll()
{
    ActiveCamera->recalculateViewMatrix();

    for (std::list<IRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
        (* it)->draw(* ActiveCamera);
}

IRenderable * const CSceneManager::pickRenderable(SLine3 const & ViewLine)
{
    return 0;
}

CCamera * const CSceneManager::getActiveCamera()
{
    return ActiveCamera;
}

void CSceneManager::setActiveCamera(CCamera * const activeCamera)
{
    ActiveCamera = activeCamera;
}
