#include "CCollisionObject.h"


CCollisionObject::CCollisionObject()
{
	CollisionType = INTERACTOR_BLOCKS; 
	CollisionMask = INTERACTOR_ALL;
}

CCollisionObject::~CCollisionObject()
{}

SVec2 CCollisionObject::performMovement(CollisionReal const TickTime)
{
	return SVec2();
}
