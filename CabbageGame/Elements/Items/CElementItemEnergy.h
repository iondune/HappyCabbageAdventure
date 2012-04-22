#ifndef CELEMENTITEMENERGY_H_
#define CELEMENTITEMENERGY_H_

#include "CElementItem.h"

class CElementItemEnergy : public CElementItem
{
   private:

   public:
      CElementItemEnergy(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void printInformation();
};

#endif
