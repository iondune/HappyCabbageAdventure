#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>

#include "CObject.h"
#include "CActor.h"

namespace Cabbage
{
namespace Collider
{

	class CEngine
	{

		typedef std::vector<CObject *> ObjectList;
		ObjectList Objects;

		typedef std::vector<CActor *> ActorList;
		ActorList Actors;

		void performTick(float const TickTime)
		{
			// Perform actor update
			// 
			for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
			{
				(* it)->updateVectors(TickTime);

				for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
					(* it)->checkCollision(* jt, TickTime);
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

		float Timer;

	public:

		CEngine()
			: Timer(0.f)
		{}

		~CEngine()
		{}

		void updateAll(float const Elapsed)
		{
			static int const TicksPerSecond = 200;
			Timer += Elapsed;

			float const TimePerTick = 1.f / TicksPerSecond;

			while (Timer > TimePerTick)
			{
				Timer -= TimePerTick;
				performTick(TimePerTick);
			}
		}

		CObject * addObject()
		{
			Objects.push_back(new CObject());
			return Objects.back();
		}

		CActor * addActor()
		{
			Actors.push_back(new CActor());
			return Actors.back();
		}

	};

}
}


#endif
