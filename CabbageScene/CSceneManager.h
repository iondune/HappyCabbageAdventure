#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "CRenderable.h"

#include "../CabbageCore/SLine3.h"


class CSceneManager
{

    std::list<CRenderable *> Renderables;

    CCamera * ActiveCamera;
    CCamera DefaultCamera;

public:

    CSceneManager();

    void addRenderable(CRenderable * Renderable);
    void removeRenderable(CRenderable * Renderable);

    void drawAll();

    CRenderable * const pickRenderable(SLine3 const & ViewLine);

    CCamera * const getActiveCamera();
    void setActiveCamera(CCamera * const activeCamera);

};

#endif
