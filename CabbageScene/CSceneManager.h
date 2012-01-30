#ifndef _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEMANAGER_H_INCLUDED_

#include <list>

#include "IRenderable.h"

#include "../CabbageCore/SLine3.h"


class CSceneManager
{

    std::list<IRenderable *> Renderables;

public:

    CSceneManager();

    void addRenderable(IRenderable * Renderable);
    void removeRenderable(IRenderable * Renderable);

    void drawAll();

    IRenderable * const pickRenderable(SLine3 const & ViewLine);

};

#endif
