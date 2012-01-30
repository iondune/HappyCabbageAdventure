#include "CLevelManager.h"

#include "../CabbageScene/CMeshLoader.h"
#include "../CabbageScene/CMeshRenderable.h"

CLevelManager::CLevelManager(CSceneManager * sceneManager)
    : SceneManager(sceneManager)
{
}

void CLevelManager::addBlockObject(SVector2 const & Position, SVector2 const & Size, int const Type)
{
    CMesh * Mesh = CMeshLoader::createCubeMesh();
    IRenderable * Renderable = new CMeshRenderable(* Mesh);

    Renderable->setScale(SVector3(Size.X, Size.Y, 1.f));
    Renderable->setTranslation(SVector3(Position.X, Position.Y, 0));

    SceneManager->addRenderable(Renderable);
}
