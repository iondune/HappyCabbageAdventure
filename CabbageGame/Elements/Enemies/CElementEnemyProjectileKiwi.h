#ifndef ____CELEMENTENEMYPROJECTILEGRAPE_HEADER_
#define ____CELEMENTENEMYPROJECTILEGRAPE_HEADER_

#include "CElementEnemyProjectile.h"

class CElementEnemyProjectileKiwi : public CElementEnemyProjectile {

   public:
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      CElementEnemyProjectileKiwi(SRect2 nArea);

      virtual void printInformation();
};
#endif
