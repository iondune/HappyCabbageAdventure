#include "CNonCollideableObject.h"

namespace Cabbage
{
   namespace Collider
   {

      CNonCollideableObject::CNonCollideableObject()
      {
         CollideableType = COLLIDEABLE_TYPE_BLOCK;
      }

      CNonCollideableObject::~CNonCollideableObject()
      {}

      SVector2 CNonCollideableObject::performMovement(float const TickTime)
      {
         return SVector2();
      }
   }
}
