#include "CCollisionElevator.h"


CCollisionElevator::CCollisionElevator()
{
	CollisionMask = INTERACTOR_ALL;
	Timer = 0.f;
	Speed = 0.5f;
	Range = 0.5f;
	InitialDirection = 1.f;
}

SVector2f CCollisionElevator::performMovement(float const TickTime)
{
	SVector2f Movement;
	Timer += TickTime * Speed;
	Movement.X = InitialDirection * TickTime * sin(Timer) * Range;
	Area.Position += Movement;
	return Movement;
}
