#ifndef _CABBAGECOLLIDER_CAPPLICATION_H_INCLUDED_
#define _CABBAGECOLLIDER_CAPPLICATION_H_INCLUDED_

#include "../CabbageCore/CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"

#include "CEventManager.h"
#include "CStateManager.h"

class CApplication
{

    // Managers
    CEventManager * EventManager;
    CStateManager * StateManager;
    CSceneManager * SceneManager;

    // Attributes
    SPosition2 WindowSize;

    // Helper functions
    void setupRenderContext();

    // Private ctor - singleton
    CApplication();

public:

    static CApplication & get();

    void init(SPosition2 const & windowSize);

    CEventManager & getEventManager();
    CStateManager & getStateManager();
    CSceneManager & getSceneManager();

    void run();

};

#endif
