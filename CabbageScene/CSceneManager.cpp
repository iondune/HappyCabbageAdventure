#include "CSceneManager.h"

#include <algorithm>


CSceneManager::CSceneManager()
    : ActiveCamera(0)
{
    ActiveCamera = & DefaultCamera;
}

void CSceneManager::addRenderable(CRenderable * Renderable)
{
    Renderables.push_back(Renderable);
}

void CSceneManager::removeRenderable(CRenderable * Renderable)
{
    printf("Removing %d (Renderables size: %d)\n", Renderable, Renderables.size());
    Renderables.erase(std::remove(Renderables.begin(), Renderables.end(), Renderable), Renderables.end());
    printf("Removed %d (Renderables size: %d)\n", Renderable, Renderables.size());
}

void CSceneManager::drawAll()
{
    ActiveCamera->recalculateViewMatrix();

    //printf("size of renderables list: %d\n", Renderables.size());
    for (std::list<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
        (* it)->draw(* ActiveCamera);
}

CRenderable * const CSceneManager::pickRenderable(SLine3 const & ViewLine)
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
