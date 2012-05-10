#ifndef CELEMENTITEMHEALTH_H_
#define CELEMENTITEMHEALTH_H_

#include "CElementItem.h"

class CElementItemHealth : public CElementItem
{
   private:

   public:
      CElementItemHealth(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object);

      virtual void printInformation();
};

#endif
