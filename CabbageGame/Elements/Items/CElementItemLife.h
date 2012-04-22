#ifndef CELEMENTITEMLIFE_H_
#define CELEMENTITEMLIFE_H_

#include "CElementItem.h"

class CElementItemLife : public CElementItem
{
   private:

   public:
      CElementItemLife(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void printInformation();
};

#endif
