#include "CCollisionEngine.h"

bool CCollisionEngine::CanCollide( CCollideable *a, CCollideable *b )
{
	return (a->CollideableLevel & b->CanCollideWith) ||
		(b->CollideableLevel & a->CanCollideWith);
}

std::vector<CCollideable*> KillList;
std::vector<CCollisionActor *> AddList;

void CCollisionEngine::performTick( float const TickTime )
{
	// Perform actor update
	// 
	int numThings = Actors.size() + Objects.size();
	//printf("There are %d things\n", numThings);
	for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
	{
		(* it)->updateVectors(TickTime);

		CCollideable * Which = 0;

		for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
		{
			if(CanCollide((*it), (*jt))) {
				//printf("%d and %d are within 2\n", (*it)->CollideableLevel, (*jt)->CollideableLevel);
				bool Alighted = (* it)->updateCollision(* jt, TickTime);
				if (Alighted)
					Which = (* jt);
			}
		}

		for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
		{
			if(CanCollide((*it), (*jt))) {
				//printf("%d and %d are within 2\n", (*it)->CollideableLevel, (*jt)->CollideableLevel);
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
		SVector2 Movement = (* it)->performMovement(TickTime);

		for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
		{
			(* jt)->pushIfCollided(* it, Movement);
		}
	}

   for(int i = 0; i < KillList.size(); i++) {
      CCollideable *Object = KillList[i];
      for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
         if (* it == Object)
         {
            Actors.erase(it);
            return;
         }
      for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
         if (* it == Object )
         {
            Objects.erase(it);
            return;
         }
   }
   KillList.clear();
   for(int i = 0; i < AddList.size(); i++) {
      Actors.push_back(AddList[i]);
   }
   AddList.clear();
}

void CCollisionEngine::removeAll()
{
	Objects.clear();
	addNullBlock();
	Actors.clear();
}

void CCollisionEngine::removeObject( CCollisionObject * Object )
{
	for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
		if (* it == Object )
		{
			Objects.erase(it);
			return;
		}
}

CCollisionEngine::CCollisionEngine() : Timer(0.f)
{
	addNullBlock();
}

CCollisionEngine::~CCollisionEngine()
{

}

void CCollisionEngine::remove(CCollideable * Object) {
   KillList.push_back(Object);
}


void CCollisionEngine::removeActor( CCollisionActor * Actor )
{
	for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
		if (* it == Actor)
		{
			Actors.erase(it);
			return;
		}
}

void CCollisionEngine::update(float const Elapsed) {
   updateAll(Elapsed);
}

void CCollisionEngine::updateAll( float const Elapsed )
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

CCollisionObject* const CCollisionEngine::getObjectBelow( SVector2 pos )
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

float const CCollisionEngine::getHeightBelow( SVector2 pos )
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

void CCollisionEngine::addNullBlock()
{
	CCollisionObject * nullBlock = this->addObject();
	nullBlock->setArea(SRect2(-1000.f, -1000.f, 0.01f, 0.01f)); 
	nullBlock->CollideableLevel = INTERACTOR_NULL_BLOCK; 
	nullBlock->CanCollideWith = INTERACTOR_ALL_ALL;
}

CCollisionObject * CCollisionEngine::addObject()
{
	CCollisionObject * a;
	Objects.push_back(a = new CCollisionObject());
	return Objects.back();
}

CElevator * CCollisionEngine::addElevator()
{
	CElevator * cen;
	Objects.push_back(cen = new CElevator());
	return cen;
}


CCollisionActor * CCollisionEngine::addActor()
{
	CCollisionActor *a;
	a = new CCollisionActor();
   AddList.push_back(a);
	return a;
}

ObjectList const & CCollisionEngine::getObjects() const
{
	return Objects;
}

ActorList const & CCollisionEngine::getActors() const
{
	return Actors;
}

