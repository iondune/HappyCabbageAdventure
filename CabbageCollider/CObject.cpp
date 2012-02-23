#include "CObject.h"

namespace Cabbage
{
   namespace Collider
   {

      CObject::CObject()
      {
         CollideableType = COLLIDEABLE_TYPE_BLOCK;
      }

      CObject::~CObject()
      {}

      SVector2 CObject::performMovement(float const TickTime)
      {
         return SVector2();
      }

      CElevator::CElevator()
      {
         CollideableType = COLLIDEABLE_TYPE_ELEVATOR;
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

   }
}
