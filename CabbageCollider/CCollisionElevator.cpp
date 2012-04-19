#include "CCollisionElevator.h"


CElevator::CElevator()
{
	CollisionMask = INTERACTOR_ALL;
	Timer = 0.f;
	Speed = 0.5f;
	Range = 0.5f;
	InitialDirection = 1.f;
}

SVector2 CElevator::performMovement(float const TickTime)
{
	SVector2 Movement;
	Timer += TickTime * Speed;
	Movement.X = InitialDirection * TickTime * sin(Timer) * Range;
	Area.Position += Movement;
	return Movement;
}
