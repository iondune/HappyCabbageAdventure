#include "CCollisionEngine.h"

#include <algorithm>

void CCollisionEngine::performTick(float const TickTime)
{
	for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
	{
		(* it)->updateVectors(TickTime);

		CCollideable * Which = 0;

		for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
		{
			if ((* it)->canCollideWith(* jt) || (* it)->canDetectWith(* jt))
			{
				bool Alighted = (* it)->updateCollision(* jt, TickTime);
				if (Alighted)
					Which = (* jt);
			}
		}

		for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
		{
			if ((* it)->canCollideWith(* jt) || (* it)->canDetectWith(* jt))
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
		SVector2<double> Movement = (* it)->performMovement(TickTime);

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

void CCollisionEngine::remove(CCollideable * Collideable)
{
	RemoveList.insert(Collideable);
}

void CCollisionEngine::removeCollideable(CCollideable * Collideable)
{
	RemoveList.insert(Collideable);
}

CCollisionEngine::CCollisionEngine()
	: Timer(0.f)
{}

CCollisionEngine::~CCollisionEngine()
{}

void CCollisionEngine::runLists()
{
	for (CollideableList::iterator it = RemoveList.begin(); it != RemoveList.end(); ++ it)
	{
		Objects.erase((CCollisionObject *) * it);
		Actors.erase((CCollisionActor *) * it);
	}

	RemoveList.clear();

	for (ActorList::iterator it = ActorAddList.begin(); it != ActorAddList.end(); ++ it)
		Actors.insert(* it);

	ActorAddList.clear();

	for (ObjectList::iterator it = ObjectAddList.begin(); it != ObjectAddList.end(); ++ it)
		Objects.insert(* it);

	ObjectAddList.clear();
}

void CCollisionEngine::update(float const Elapsed)
{
   updateAll(Elapsed);
}

void CCollisionEngine::updateAll(float const Elapsed)
{
	static int const TicksPerSecond = 200;

	runLists();

	Timer += std::min(Elapsed, 0.1f);

	float const TimePerTick = 1.f / TicksPerSecond;

	while (Timer > TimePerTick)
	{
		Timer -= TimePerTick;
		performTick(TimePerTick);
	}

	runLists();
}

std::vector<CCollideable *> const CCollisionEngine::getAllInBound(SRect2f const Bound) const
{
	std::vector<CCollideable *> toReturn;
	for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
	{
		if (Bound.intersects((*jt)->getArea()))
			toReturn.push_back((*jt));
	}

	for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
	{
		if (Bound.intersects((*jt)->getArea()))
			toReturn.push_back((*jt));
	}
	return toReturn;
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
			height = (float) Object->getArea().otherCorner().Y;
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
			height = (float) Object->getArea().otherCorner().Y;
	}

	return height;
}

float const CCollisionEngine::getHeightBelow( CCollisionActor * Actor )
{
	float height = - std::numeric_limits<float>::infinity();
	float checkHeight;

	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
	{
		if ((* it)->canCollideWith(Actor) && Actor->isAbove(* it, checkHeight))
			if (checkHeight > height)
				height = checkHeight;
	}

	return height;
}

CCollisionObject * CCollisionEngine::addObject()
{
	CCollisionObject * a;
	ObjectAddList.insert(a = new CCollisionObject());
	return a;
}

CCollisionElevator * CCollisionEngine::addElevator()
{
	CCollisionElevator * cen;
	ObjectAddList.insert(cen = new CCollisionElevator());
	return cen;
}

CCollisionActor * CCollisionEngine::addActor()
{
	CCollisionActor *a;
	ActorAddList.insert(a = new CCollisionActor());
	return a;
}

CCollisionEngine::ObjectList const & CCollisionEngine::getObjects() const
{
	return Objects;
}

CCollisionEngine::ActorList const & CCollisionEngine::getActors() const
{
	return Actors;
}
