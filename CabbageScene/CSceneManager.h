#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CRenderable.h"

#include "../CabbageCore/SLine3.h"
#include "../CabbageCore/SColor.h"


class CLight
{
	// CLight should implement ISceneObject
	// that way light billboards will be drawable by using a scene->set debug flag
	// and positioning,etc will be inheritted
	SUniform<SColor> BindColor;
	SUniform<SVector3> BindPosition;

public:

    SColor Color;
    SVector3 Position;

    // Todo: change values only through get/set, set scene changed when so

    CLight()
		: BindColor(Color), BindPosition(Position)
    {}
};

class CScene
{

    static CLight const NullLight;

protected:

    CCamera DefaultCamera;
    CCamera * ActiveCamera;

    glm::mat4 ViewMatrix, ProjMatrix;
    int LightCount;

	SUniform<glm::mat4> BindViewMatrix, BindProjMatrix;
	SUniform<int> BindLightCount;

    std::map<std::string, IUniform const *> Uniforms;

public:

    CScene();

    CCamera * const getActiveCamera();
    void setActiveCamera(CCamera * const activeCamera);

    void addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform);
    void removeUniform(std::string const & label);

    IUniform const * getUniform(std::string const & label) const;

    void update();

    std::vector<CLight *> Lights;
    bool SceneChanged;

};

class CSceneManager : public CScene
{

    std::list<CRenderable *> Renderables;

    CScene * CurrentScene;

public:

    CSceneManager();

    void addRenderable(CRenderable * Renderable);
    void removeRenderable(CRenderable * Renderable);
    void removeAllRenderables();

    void drawAll();

    CRenderable * const pickRenderable(SLine3 const & ViewLine);

};

#endif
