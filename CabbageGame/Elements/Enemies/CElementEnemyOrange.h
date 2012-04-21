#ifndef CELEMENTENEMYORANGE_H_
#define CELEMENTENEMYORANGE_H_

#include "CElementEnemy.h"

class CElementEnemyOrange : public CElementEnemy
{
   private:
      bool Jumped;

   public:
      CElementEnemyOrange(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object, CCollideable *With);
};

#endif
