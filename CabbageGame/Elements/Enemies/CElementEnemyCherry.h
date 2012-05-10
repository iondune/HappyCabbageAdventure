#ifndef CELEMENTENEMYCHERRY_H_
#define CELEMENTENEMYCHERRY_H_

#include "CElementEnemy.h"

class CElementEnemyCherry : public CElementEnemy
{
   private:
      SVector2f Scale;
      float BombTime;

   public:
      CElementEnemyCherry(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
