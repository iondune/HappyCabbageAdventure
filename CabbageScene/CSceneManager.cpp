#include "CSceneManager.h"

#include <algorithm>
#include <sstream>


CLight const CScene::NullLight;


CScene::CScene()
	: BindProjMatrix(ProjMatrix), BindViewMatrix(ViewMatrix), BindLightCount(LightCount)
{
    ActiveCamera = & DefaultCamera;

    addUniform("uProjMatrix", & BindProjMatrix);
    addUniform("uViewMatrix", & BindViewMatrix);
    addUniform("uLightCount", & BindLightCount);
}

void CScene::addUniform(std::string const & label, IUniform const * uniform)
{
    Uniforms[label] = uniform;
}

void CScene::removeUniform(std::string const & label)
{
    std::map<std::string, IUniform const *>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        Uniforms.erase(it);
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

IUniform const * CScene::getUniform(std::string const & label) const
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
				return & NullLight.BindColor;
            else
                return & Lights[index]->BindColor;
        }
        else if (remaining == "Position")
        {
            if (index >= Lights.size())
                return & NullLight.BindPosition;
            else
                return & Lights[index]->BindPosition;
        }
    }

    std::map<std::string, IUniform const *>::const_iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        return it->second;

    return 0;
}

void CScene::update()
{
    ActiveCamera->recalculateViewMatrix();
    ViewMatrix = ActiveCamera->getViewMatrix();
    ProjMatrix = ActiveCamera->getProjectionMatrix();

    if (LightCount != Lights.size())
    {
        SceneChanged = true;
        LightCount = Lights.size();
    }
}

CSceneManager::CSceneManager()
{
    CurrentScene = this;
}

void CSceneManager::addSceneObject(CSceneObject * sceneObject)
{
	SceneObjects.push_back(sceneObject);
}

void CSceneManager::removeSceneObject(CSceneObject * sceneObject)
{
	SceneObjects.erase(std::remove(SceneObjects.begin(), SceneObjects.end(), sceneObject), SceneObjects.end());
}

void CSceneManager::removeAllSceneObjects()
{
	SceneObjects.erase(SceneObjects.begin(), SceneObjects.end());
}

void CSceneManager::drawAll()
{
    CurrentScene->update();

    //printf("size of renderables list: %d\n", Renderables.size());
    for (std::list<CSceneObject *>::iterator it = SceneObjects.begin(); it != SceneObjects.end(); ++ it)
        (* it)->draw(CurrentScene);

    SceneChanged = false;
}
