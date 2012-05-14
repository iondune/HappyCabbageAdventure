#ifndef ____CELEMENTENEMYPROJECTILEKIWI_HEADER_
#define ____CELEMENTENEMYPROJECTILEKIWI_HEADER_

#include "CElementEnemyProjectile.h"

class CElementEnemyProjectileKiwi : public CElementEnemyProjectile {

   public:
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object);

      CElementEnemyProjectileKiwi(SRect2 nArea);

      virtual void printInformation();
};
#endif
