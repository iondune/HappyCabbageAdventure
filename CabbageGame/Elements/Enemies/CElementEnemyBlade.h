#ifndef CELEMENTENEMYBLADE_H_
#define CELEMENTENEMYBLADE_H_

#include "CElementEnemy.h"

class CElementEnemyBlade : public CElementEnemy
{
   private:
      float BladeRotate;

   public:
      CElementEnemyBlade(SRect2 nArea);

      virtual void setupObjects();
      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
};

#endif
