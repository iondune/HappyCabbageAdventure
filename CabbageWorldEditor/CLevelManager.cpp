#include "CLevelManager.h"

#include "../CabbageScene/CMeshLoader.h"
#include "../CabbageScene/CMeshRenderable.h"
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
    CMeshRenderable * Renderable = new CMeshRenderable();
    Renderable->setMesh(Mesh);

    Renderable->setScale(SVector3(Size.X, Size.Y, 1.f));
    Renderable->setTranslation(SVector3(Position.X, Position.Y, 0));

    Renderable->getMaterial().Shader = CShaderLoader::loadShader("Flat");

    SceneManager->addRenderable(Renderable);
}
