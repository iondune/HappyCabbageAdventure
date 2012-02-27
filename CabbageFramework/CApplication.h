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

    float ElapsedTime;

    unsigned int Time0, Time1;

	bool Running;

public:

    static CApplication & get();

    void init(SPosition2 const & windowSize);

    CEventManager & getEventManager();
    CStateManager & getStateManager();
    CSceneManager & getSceneManager();

    void run();

    float const getElapsedTime() const;
    float const getRunTime() const;

    SPosition2 const & getWindowSize() const;

    void skipElapsedTime();

	bool shuttingDown() const;
	void close();

};

#endif
