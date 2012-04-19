#include "CCollisionObject.h"


CCollisionObject::CCollisionObject()
{
	CollisionType = INTERACTOR_BLOCKS; 
	CollisionMask = INTERACTOR_ALL;
}

CCollisionObject::~CCollisionObject()
{}

SVector2 CCollisionObject::performMovement(float const TickTime)
{
	return SVector2();
}
