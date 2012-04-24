#include "CCollisionObject.h"


CCollisionObject::CCollisionObject()
{
	CollisionType = INTERACTOR_BLOCKS; 
	CollisionMask = INTERACTOR_ALL;
}

CCollisionObject::~CCollisionObject()
{}

SVector2f CCollisionObject::performMovement(float const TickTime)
{
	return SVector2();
}
