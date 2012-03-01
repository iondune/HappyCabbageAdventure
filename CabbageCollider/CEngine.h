#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <limits>

#include "CObject.h"
#include "CActor.h"

#define WITHIN(x,y,z) ((x)>(y)?((x)-(y)<(z)?1:0):((y)-(x)<(z)?1:0))

namespace Cabbage
{
   namespace Collider
   {

      class ICollisionResponder
      {

         public:

            virtual void OnCollision(CCollideable * Object, CCollideable * With) =0;

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

            void performTick(float const TickTime)
            {
               // Perform actor update
               // 
               for (ActorList::iterator it = Actors.begin(); it != Actors.end(); ++ it)
               {
                  (* it)->updateVectors(TickTime);

                  CCollideable * Which = 0;

                  for (ObjectList::iterator jt = Objects.begin(); jt != Objects.end(); ++ jt)
                  {
                     if(WITHIN((*it)->CollideableLevel, (*jt)->CollideableLevel, 2)) {
                        //printf("%d and %d are within 2\n", (*it)->CollideableLevel, (*jt)->CollideableLevel);
                        bool Alighted = (* it)->updateCollision(* jt, TickTime, CollisionResponder);
                        if (Alighted)
                           Which = (* jt);
                     }
                  }

                  for (ActorList::iterator jt = Actors.begin(); jt != Actors.end(); ++ jt)
                  {
                     if(WITHIN((*it)->CollideableLevel, (*jt)->CollideableLevel, 2)) {
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

            float Timer;

         public:

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
            {}

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

            float const getHeightBelow(CActor * Actor)
            {
               float height = - std::numeric_limits<float>::infinity();
               float checkHeight;

               for (ObjectList::iterator it = Objects.begin(); it != Objects.end(); ++ it)
               {
                  if (Actor->CollideableLevel >= (*it)->CollideableLevel && Actor->isAbove(* it, checkHeight))
                     if (checkHeight > height)
                        height = checkHeight;
               }

               return height;
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
   }
}

#endif
