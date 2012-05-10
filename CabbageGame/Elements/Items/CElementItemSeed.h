#ifndef CELEMENTITEMSEED_H_
#define CELEMENTITEMSEED_H_

#include "CElementItem.h"

class CElementItemSeed : public CElementItem
{
   private:

   public:
      CElementItemSeed(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object);

      virtual void printInformation();
};

#endif
