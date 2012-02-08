#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CRenderable.h"

#include "../CabbageCore/SLine3.h"
#include "../CabbageCore/SColor.h"


struct SLight
{
    SVector3 Position;
    SColor Color;
};

class CScene
{

protected:

    CCamera DefaultCamera;
    CCamera * ActiveCamera;

    boost::shared_ptr<CMat4Uniform> uViewMatrix, uProjMatrix;

    std::map<std::string, CRenderable::SUniform> Uniforms;

public:

    CScene();

    CCamera * const getActiveCamera();
    void setActiveCamera(CCamera * const activeCamera);

    void addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform);
    void removeUniform(std::string const & label);

    std::map<std::string, CRenderable::SUniform> & getUniforms();
    std::map<std::string, CRenderable::SUniform> const & getUniforms() const;

    void update();

    std::vector<SLight> Lights;

};

class CSceneManager : public CScene
{

    std::list<CRenderable *> Renderables;

    CScene * CurrentScene;

public:

    CSceneManager();

    void addRenderable(CRenderable * Renderable);
    void removeRenderable(CRenderable * Renderable);

    void drawAll();

    CRenderable * const pickRenderable(SLine3 const & ViewLine);

};

#endif
