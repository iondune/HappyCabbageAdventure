#ifndef _CABBAGECOLLIDER_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_COBJECT_H_INCLUDED_

#include "CCollideable.h"
#include "../CabbageScene/CSceneObject.h"

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
         CSceneObject *Renderable;

         public:

         CElevator();

         virtual SVector2 performMovement(float const TickTime);

         CSceneObject * getRenderable();
         void setRenderable(CSceneObject *);
         float Speed;
 
         float InitialDirection;

         float Range;
      };
   }
}

#endif
