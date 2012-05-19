#ifndef CELEMENTENEMYPear_H_
#define CELEMENTENEMYPear_H_

#include "CElementEnemy.h"

class CElementEnemyPear : public CElementEnemy, public ISquishable
{
   private:
      SVector2f Scale;
      float OldPositionX;
      bool HitPlayer;

   public:
      CElementEnemyPear(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void OnCollision(const SCollisionEvent& Event);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
