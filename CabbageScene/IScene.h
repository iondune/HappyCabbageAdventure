#ifndef _CABBAGE_SCENE_ISCENE_H_INCLUDED_
#define _CABBAGE_SCENE_ISCENE_H_INCLUDED_

#include "ICameraSceneObject.h"

class IScene
{

public:

	virtual ICameraSceneObject * const getActiveCamera() = 0;
	virtual ICameraSceneObject const * const getActiveCamera() const = 0;
	virtual void setActiveCamera(ICameraSceneObject * const activeCamera) = 0;

};

#endif
