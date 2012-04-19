#include "CCollisionObject.h"

CCollisionObject::CCollisionObject()
{
	CollideableType = COLLIDEABLE_TYPE_BLOCK;
	CollideableLevel = INTERACTOR_BLOCKS; 
	CanCollideWith = INTERACTOR_ALL;
}

CCollisionObject::~CCollisionObject()
{}

SVector2 CCollisionObject::performMovement(float const TickTime)
{
	return SVector2();
}

CElevator::CElevator()
{
	CollideableType = COLLIDEABLE_TYPE_ELEVATOR;
	CollideableLevel = INTERACTOR_ALL;
	Timer = 0.f;
	Speed = 0.5f;
	Range = 0.5f;
	InitialDirection = 1.f;
}


void CElevator::setRenderable(CSceneObject *r) {
	Renderable = r;
}

CSceneObject * CElevator::getRenderable() {
	return Renderable;
}


SVector2 CElevator::performMovement(float const TickTime)
{
	SVector2 Movement;
	Timer += TickTime * Speed;
	Movement.X = InitialDirection * TickTime * sin(Timer) * Range;
	Area.Position += Movement;
	return Movement;
}

