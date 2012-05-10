#ifndef CELEMENTITEMENERGY_H_
#define CELEMENTITEMENERGY_H_

#include "CElementItem.h"

class CElementItemEnergy : public CElementItem
{
   private:

   public:
      CElementItemEnergy(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object);

      virtual void printInformation();
};

#endif
