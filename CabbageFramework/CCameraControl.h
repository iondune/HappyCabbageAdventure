#pragma once

#include "SVector3.h"
#include "SVector2.h"

#include "../CabbageScene/CabbageScene.h"
#include "CEventManager.h"
#include "CApplication.h"

class CCameraControl : public CPerspectiveCameraSceneObject, public sigslot::has_slots<>
{

    CApplication & Application;
    CEventManager & EventManager;

    
    float MoveSpeed;

    int MouseLastX, MouseLastY;

    bool Tracking;


public:

    float Phi, Theta;

	CCameraControl(SVector3f const position = SVector3f(0, 3, 2));

	void OnMouseEvent(SMouseEvent const & Event);

	void update(float const TickTime);

	SVector3f const & getPosition();

};
