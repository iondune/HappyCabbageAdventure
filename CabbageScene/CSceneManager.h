#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CRenderable.h"

#include "../CabbageCore/SLine3.h"
#include "../CabbageCore/SColor.h"


struct SLight
{
    boost::shared_ptr<CVec3Uniform> ColorUniform;
    boost::shared_ptr<CVec3Uniform> PositionUniform;

    // Todo: change values only through get/set, set scene changed when so

    SLight()
    {
        ColorUniform = boost::shared_ptr<CVec3Uniform>(new CVec3Uniform());
        PositionUniform = boost::shared_ptr<CVec3Uniform>(new CVec3Uniform());
    }
};

class CScene
{

    static SLight const NullLight;

protected:

    CCamera DefaultCamera;
    CCamera * ActiveCamera;

    boost::shared_ptr<CMat4Uniform> uViewMatrix, uProjMatrix;
    boost::shared_ptr<CIntUniform> uLightCount;

    std::map<std::string, CRenderable::SUniform> Uniforms;

public:

    CScene();

    CCamera * const getActiveCamera();
    void setActiveCamera(CCamera * const activeCamera);

    void addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform);
    void removeUniform(std::string const & label);

    std::map<std::string, CRenderable::SUniform> & getExplicitUniforms();
    std::map<std::string, CRenderable::SUniform> const & getExplicitUniforms() const;

    boost::shared_ptr<IUniform> const getUniform(std::string const & label) const;

    void update();

    std::vector<SLight> Lights;
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
