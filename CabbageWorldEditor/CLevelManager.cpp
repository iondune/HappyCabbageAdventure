#include "CLevelManager.h"

#include "../CabbageScene/CMeshLoader.h"
#include "../CabbageScene/CMeshSceneObject.h"
#include "../CabbageScene/CShaderLoader.h"

CLevelManager::CLevelManager(CSceneManager * sceneManager)
    : SceneManager(sceneManager)
{
}

void CLevelManager::addBlockObject(SVector2 Position, SVector2 Size, int const Type)
{
    Position *= 0.1f;
    Size *= 0.1f;

    CMesh * Mesh = CMeshLoader::createCubeMesh();
    CMeshSceneObject * Renderable = new CMeshSceneObject();
    Renderable->setMesh(Mesh);

    Renderable->setScale(SVector3f(Size.X, Size.Y, 1.f));
    Renderable->setTranslation(SVector3f(Position.X, Position.Y, 0));

    Renderable->setShader(CShaderLoader::loadShader("Flat"));

    SceneManager->addSceneObject(Renderable);
}
