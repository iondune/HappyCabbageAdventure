#ifndef _CABBAGECOLLIDER_CLEVELMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CLEVELMANAGER_H_INCLUDED_

#include "../CabbageCore/SVector2.h"
#include "../CabbageScene/CSceneManager.h"

class CLevelManager
{

    CSceneManager * SceneManager;

public:

    CLevelManager(CSceneManager * sceneManager);

    void addBlockObject(SVector2 const & Position, SVector2 const & Size, int const Type);

};

#endif
