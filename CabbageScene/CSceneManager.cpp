#include "CSceneManager.h"

#include <algorithm>


CScene::CScene()
{
    ActiveCamera = & DefaultCamera;

    uProjMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());
    uViewMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());

    addUniform("uProjMatrix", uProjMatrix);
    addUniform("uViewMatrix", uViewMatrix);
}

void CScene::addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform)
{
    Uniforms[label] = CRenderable::SUniform(uniform);
}

void CScene::removeUniform(std::string const & label)
{
    std::map<std::string, CRenderable::SUniform>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        Uniforms.erase(it);
}

std::map<std::string, CRenderable::SUniform> & CScene::getUniforms()
{
    return Uniforms;
}

std::map<std::string, CRenderable::SUniform> const & CScene::getUniforms() const
{
    return Uniforms;
}

CCamera * const CScene::getActiveCamera()
{
    return ActiveCamera;
}

void CScene::setActiveCamera(CCamera * const activeCamera)
{
    ActiveCamera = activeCamera;
}

void CScene::update()
{
    ActiveCamera->recalculateViewMatrix();
    uViewMatrix->Value = ActiveCamera->getViewMatrix();
    uProjMatrix->Value = ActiveCamera->getProjectionMatrix();
}


CSceneManager::CSceneManager()
{
    CurrentScene = this;
}

void CSceneManager::addRenderable(CRenderable * Renderable)
{
    Renderables.push_back(Renderable);
}

void CSceneManager::removeRenderable(CRenderable * Renderable)
{
    Renderables.erase(std::remove(Renderables.begin(), Renderables.end(), Renderable), Renderables.end());
}

void CSceneManager::drawAll()
{
    CurrentScene->update();

    //printf("size of renderables list: %d\n", Renderables.size());
    for (std::list<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
        (* it)->draw(CurrentScene);
}

CRenderable * const CSceneManager::pickRenderable(SLine3 const & ViewLine)
{
    return 0;
}
