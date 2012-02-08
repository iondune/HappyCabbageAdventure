#include "CSceneManager.h"

#include <algorithm>
#include <sstream>


SLight const CScene::NullLight;


CScene::CScene()
{
    ActiveCamera = & DefaultCamera;

    uProjMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());
    uViewMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());
    uLightCount = boost::shared_ptr<CIntUniform>(new CIntUniform());

    addUniform("uProjMatrix", uProjMatrix);
    addUniform("uViewMatrix", uViewMatrix);
    addUniform("uLightCount", uLightCount);
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

std::map<std::string, CRenderable::SUniform> & CScene::getExplicitUniforms()
{
    return Uniforms;
}

std::map<std::string, CRenderable::SUniform> const & CScene::getExplicitUniforms() const
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

unsigned int const digitCount(int n)
{
    int count = 1;
    if (n < 0)
    {
        n *= -1;
        ++ count;
    }

    while (n > 10)
    {
        ++ count;
        n /= 10;
    }

    return count;
}

boost::shared_ptr<IUniform> const CScene::getUniform(std::string const & label) const
{
    if (label.substr(0, 8) == "uLights[")
    {
        std::stringstream ss(label.substr(8));
        unsigned int index;
        ss >> index;
        std::string remaining = ss.str();
        remaining = remaining.substr(2 + digitCount(index));

        if (remaining == "Color")
        {
            if (index >= Lights.size())
                return NullLight.ColorUniform;
            else
                return Lights[index].ColorUniform;
        }
        else if (remaining == "Position")
        {
            if (index >= Lights.size())
                return NullLight.PositionUniform;
            else
                return Lights[index].PositionUniform;
        }
    }

    std::map<std::string, CRenderable::SUniform>::const_iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        return it->second.Value;

    return boost::shared_ptr<IUniform>();
}

void CScene::update()
{
    ActiveCamera->recalculateViewMatrix();
    uViewMatrix->Value = ActiveCamera->getViewMatrix();
    uProjMatrix->Value = ActiveCamera->getProjectionMatrix();
    if (uLightCount->Value != Lights.size())
    {
        SceneChanged = true;
        uLightCount->Value = Lights.size();
    }
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
