#include "CSceneManager.h"

#include <algorithm>


CSceneManager::CSceneManager()
{
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
    for (std::list<IRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
        (* it)->draw();
}

IRenderable * const CSceneManager::pickRenderable(SLine3 const & ViewLine)
{
    return 0;
}
