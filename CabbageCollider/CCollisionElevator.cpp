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
	if (! Range)
		return SVec2();

	SVec2 NeededPosition;
	Timer += TickTime * Speed / Range;

	switch (Style)
	{
	case ELT_HORIZONTAL:
		NeededPosition.X = InitialDirection * (1.f - (1.f + cos(Timer)) / 2.f) * Range;
		break;
		
	case ELT_VERTICAL:
		NeededPosition.Y = InitialDirection * (1.f - (1.f + cos(Timer)) / 2.f) / 2.f * Range;
		break;

	case ELT_CIRCULAR:
		NeededPosition.X = InitialDirection * (1.f - (1.f + cos(Timer)) / 2.f) / 2.f * Range;
		NeededPosition.Y = InitialDirection * (1.f - (1.f + sin(Timer)) / 2.f) / 2.f * Range;
		break;
	}

	SVec2 Movement = (NeededPosition + BasisArea) - Area.Position;
	Area.Position += Movement;
	return Movement;
}

void CCollisionElevator::setArea(SRect2f const & area)
{
	BasisArea = area.Position;
	Area = SArea(area.Position, area.Size);
}

void CCollisionElevator::setArea(SRect2d const & area)
{
	BasisArea = area.Position;
	Area = area;
}
