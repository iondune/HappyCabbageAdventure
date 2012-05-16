#ifndef _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CELEVATOR_H_INCLUDED_

#include "CCollisionObject.h"

enum EElevatorType
{
	ELT_HORIZONTAL,
	ELT_VERTICAL,
	ELT_CIRCULAR
};

class CCollisionElevator : public CCollisionObject
{

	CollisionReal Timer;

public:

	CCollisionElevator();

	virtual SVec2 performMovement(CollisionReal const TickTime);

	CollisionReal Speed;

	CollisionReal InitialDirection;

	CollisionReal Range;

	EElevatorType Style;

};

#endif
