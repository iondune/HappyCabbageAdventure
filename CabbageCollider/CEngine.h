#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <limits>

#include "CObject.h"
#include "CActor.h"

/*
#define INTERACTOR_
#define INTERACTOR_
#define INTERACTOR_
*/
#define INTERACTOR_NON 128

class ICollisionResponder
{

public:

	virtual bool OnCollision(CCollideable * Object, CCollideable * With) =0;

};

class CEngine
{

public:

	typedef std::vector<CObject *> ObjectList;
	typedef std::vector<CActor *> ActorList;

private:

	ObjectList Objects;
	ActorList Actors;

	ICollisionResponder * CollisionResponder;

	bool CanCollide(CCollideable *a, CCollideable *b);

	void performTick(float const TickTime);

	float Timer;

public:

	void removeAll() {
		Objects.clear();
		addNullBlock();
		Actors.clear();
	}

	void removeObject(CObject * Object)
	{
		for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
			if (* it == Object )
			{
				Objects.erase(it);
				return;
			}
	}


	CEngine()
		: Timer(0.f), CollisionResponder(0)
	{
		addNullBlock();
	}

	~CEngine()
	{}

	void setCollisionResponder(ICollisionResponder * collisionResponder)
	{
		CollisionResponder = collisionResponder;
	}

	void removeActor(CActor * Actor)
	{
		for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
			if (* it == Actor)
			{
				Actors.erase(it);
				return;
			}
	}

	void updateAll(float const Elapsed)
	{
		static int const TicksPerSecond = 200;
		Timer += std::min(Elapsed, 0.1f);

		float const TimePerTick = 1.f / TicksPerSecond;

		while (Timer > TimePerTick)
		{
			Timer -= TimePerTick;
			performTick(TimePerTick);
		}
	}

	CObject* const getObjectBelow(SVector2 pos)
	{
		float height = - std::numeric_limits<float>::infinity();
		CObject *Object, *objBelow = NULL;

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

	float const getHeightBelow(SVector2 pos) {
		float height = - std::numeric_limits<float>::infinity();
		CObject* Object;

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

	float const getHeightBelow(CActor * Actor)
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

	void addNullBlock() {
		CObject * nullBlock = this->addObject();
		nullBlock->setArea(SRect2(-1000.f, -1000.f, 0.01f, 0.01f)); 
		nullBlock->CollideableLevel = INTERACTOR_NULL_BLOCK; 
		nullBlock->CanCollideWith = INTERACTOR_ALL_ALL;
	}

	CObject * addObject()
	{
		CObject * a;
		Objects.push_back(a = new CObject());
		return Objects.back();
	}

	CElevator * addElevator()
	{
		CElevator * cen;
		Objects.push_back(cen = new CElevator());
		return cen; 
	}

	CActor * addActor()
	{
		CActor *a;
		Actors.push_back(a = new CActor());
		return Actors.back();
	}

	ObjectList const & getObjects() const
	{
		return Objects;
	}

	ActorList const & getActors() const
	{
		return Actors;
	}

};

#endif
