#ifndef CELEMENTITEMLIFE_H_
#define CELEMENTITEMLIFE_H_

#include "CElementItem.h"

class CElementItemLife : public CElementItem
{
   private:

   public:
      CElementItemLife(SRect2f nArea);

      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(const SCollisionEvent& Event);

      virtual void printInformation();
};

#endif
