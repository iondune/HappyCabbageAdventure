#ifndef CELEMENTENEMYGRAPE_H_
#define CELEMENTENEMYGRAPE_H_

#include "CElementEnemy.h"

class CElementEnemyGrape : public CElementEnemy
{
   private:
      float shootTime;

   public:
      CElementEnemyGrape(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void printInformation();

      void ShootGrape();
};

#endif
