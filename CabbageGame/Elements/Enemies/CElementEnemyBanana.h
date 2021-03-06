#ifndef CELEMENTENEMYBANANA_H_
#define CELEMENTENEMYBANANA_H_

#include "CElementEnemy.h"

class CElementEnemyBanana : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult, ShootTime;
      bool PositiveScale;
      SVector2f Scale;

   public:
      CElementEnemyBanana(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
