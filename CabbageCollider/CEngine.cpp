#include "CEngine.h"

void Cabbage::Collider::CEngine::performTick( float const TickTime )
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
				bool Alighted = (* it)->updateCollision(* jt, TickTime, CollisionResponder);
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
					bool Alighted = (* it)->updateCollision(* jt, TickTime, CollisionResponder);
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
}

