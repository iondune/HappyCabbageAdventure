#ifndef _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_

#include "CCollisionObject.h"

class CElevator : public CCollisionObject
{

	float Timer;

public:

	CElevator();

	virtual SVector2 performMovement(float const TickTime);

	float Speed;

	float InitialDirection;

	float Range;

};

#endif
