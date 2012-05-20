#include "CCollisionElevator.h"


CCollisionElevator::CCollisionElevator(CCollisionEngine * collisionEngine)
	: CCollisionObject(collisionEngine)
{
	CollisionMask = INTERACTOR_ALL;
	Timer = 0.f;
	Speed = 0.5f;
	Range = 0.5f;
	InitialDirection = 1.f;
	Style = ELT_HORIZONTAL;
}

SVec2 CCollisionElevator::performMovement(CollisionReal const TickTime)
{
	SVec2 Movement;
	Timer += TickTime * Speed / Range;

	switch (Style)
	{
	case ELT_HORIZONTAL:
		Movement.X = InitialDirection * TickTime * sin(Timer) * Range;
		break;
		
	case ELT_VERTICAL:
		Movement.Y = InitialDirection * TickTime * sin(Timer) * Range;
		break;

	case ELT_CIRCULAR:
		Movement.X = InitialDirection * TickTime * cos(Timer) * Range;
		Movement.Y = InitialDirection * TickTime * sin(Timer) * Range;
		break;
	}

	Area.Position += Movement;
	return Movement;
}
