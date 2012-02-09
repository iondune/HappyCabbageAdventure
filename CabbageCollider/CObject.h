#ifndef _CABBAGECOLLIDER_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_COBJECT_H_INCLUDED_

#include "CCollideable.h"
#include "../CabbageScene/CRenderable.h"

namespace Cabbage
{
   namespace Collider
   {
      class CObject : public CCollideable
      {

         protected:

            friend class CEngine;

            CObject();

         public:

            ~CObject();

            virtual SVector2 performMovement(float const TickTime);

      };

      class CElevator : public CObject
      {

         float Timer;
         CRenderable *Renderable;

         public:

         CElevator();

         virtual SVector2 performMovement(float const TickTime);

         CRenderable * getRenderable();
         void setRenderable(CRenderable *);
         float Speed;
         float Range;

      };
   }
}

#endif
