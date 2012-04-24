#include "CCollisionEngine.h"

#include <algorithm>

bool CCollisionEngine::CanCollide(CCollideable *a, CCollideable *b)
{
	return (a->CollisionType & b->CollisionMask) ||
		(b->CollisionType & a->CollisionMask);
}

void CCollisionEngine::performTick(float const TickTime)
{
	for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
	{
		(* it)->updateVectors(TickTime);

		CCollideable * Which = 0;

		for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
		{
			if(CanCollide((*it), (*jt)))
			{
				bool Alighted = (* it)->updateCollision(* jt, TickTime);
				if (Alighted)
					Which = (* jt);
			}
		}

		for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
		{
			if(CanCollide((*it), (*jt)))
			{
				if (* it != * jt)
				{
					bool Alighted = (* it)->updateCollision(* jt, TickTime);
					if (Alighted)
						Which = (* jt);
				}
			}
		}

		if (Which)
			(* it)->onStanding(Which);
	}

	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
	{
		SVector2f Movement = (* it)->performMovement(TickTime);

		for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
		{
			(* jt)->pushIfCollided(* it, Movement);
		}
	}
}

void CCollisionEngine::removeAll()
{
	Objects.clear();
	Actors.clear();
}

void CCollisionEngine::removeObject(CCollisionObject * Object)
{
	Objects.erase(std::remove(Objects.begin(), Objects.end(), Object), Objects.end());
}

CCollisionEngine::CCollisionEngine()
	: Timer(0.f)
{}

CCollisionEngine::~CCollisionEngine()
{}

void CCollisionEngine::removeActor(CCollisionActor * Actor)
{
	Actors.erase(std::remove(Actors.begin(), Actors.end(), Actor), Actors.end());
}

void CCollisionEngine::updateAll(float const Elapsed)
{
	static int const TicksPerSecond = 1000;
	Timer += std::min(Elapsed, 0.1f);

	float const TimePerTick = 1.f / TicksPerSecond;

	while (Timer > TimePerTick)
	{
		Timer -= TimePerTick;
		performTick(TimePerTick);
	}
}

CCollisionObject* const CCollisionEngine::getObjectBelow(SVector2f pos)
{
	float height = - std::numeric_limits<float>::infinity();
	CCollisionObject *Object, *objBelow = NULL;

	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
	{
		Object = *it;
		if (pos.Y < Object->getArea().otherCorner().Y)
			continue;

		if (pos.X < Object->getArea().Position.X || pos.X > Object->getArea().otherCorner().X)
			continue;

		if(Object->getArea().otherCorner().Y > height) {
			height = Object->getArea().otherCorner().Y;
			objBelow = Object;
		}
	}

	return objBelow;
}

float const CCollisionEngine::getHeightBelow(SVector2f pos)
{
	float height = - std::numeric_limits<float>::infinity();
	CCollisionObject* Object;

	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
	{
		Object = *it;
		if (pos.Y < Object->getArea().otherCorner().Y)
			continue;

		if (pos.X < Object->getArea().Position.X || pos.X > Object->getArea().otherCorner().X)
			continue;

		if(Object->getArea().otherCorner().Y > height)
			height = Object->getArea().otherCorner().Y;
	}

	return height;
}

float const CCollisionEngine::getHeightBelow( CCollisionActor * Actor )
{
	float height = - std::numeric_limits<float>::infinity();
	float checkHeight;

	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
	{
		if (CanCollide(*it, Actor) && Actor->isAbove(* it, checkHeight))
			if (checkHeight > height)
				height = checkHeight;
	}

	return height;
}

CCollisionObject * CCollisionEngine::addObject()
{
	CCollisionObject * a;
	Objects.push_back(a = new CCollisionObject());
	return Objects.back();
}

CCollisionElevator * CCollisionEngine::addElevator()
{
	CCollisionElevator * cen;
	Objects.push_back(cen = new CCollisionElevator());
	return cen;
}

CCollisionActor * CCollisionEngine::addActor()
{
	CCollisionActor *a;
	Actors.push_back(a = new CCollisionActor());
	return Actors.back();
}

CCollisionEngine::ObjectList const & CCollisionEngine::getObjects() const
{
	return Objects;
}

CCollisionEngine::ActorList const & CCollisionEngine::getActors() const
{
	return Actors;
}
