#ifndef _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_

#include "CCollisionObject.h"

class CCollisionElevator : public CCollisionObject
{

	float Timer;

public:

	CCollisionElevator();

	virtual SVector2f performMovement(float const TickTime);

	float Speed;

	float InitialDirection;

	float Range;

};

#endif
