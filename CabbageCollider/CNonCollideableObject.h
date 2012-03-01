#ifndef _CABBAGECOLLIDER_NONCOLLIDEABLE_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_NONCOLLIDEABLE_COBJECT_H_INCLUDED_

#include "CCollideable.h"
#include "../CabbageScene/CSceneObject.h"

namespace Cabbage
{
   namespace Collider
   {
      class CNonCollideableObject : public CCollideable
      {

         protected:

            friend class CEngine;

            CNonCollideableObject();

         public:

            ~CNonCollideableObject();

            virtual SVector2 performMovement(float const TickTime);
      };
   }
}

#endif
