#ifndef ____CELEMENTENEMYPROJECTILEGRAPE_HEADER_
#define ____CELEMENTENEMYPROJECTILEGRAPE_HEADER_

#include "CElementEnemyProjectile.h"

class CElementEnemyProjectileGrape : public CElementEnemyProjectile {
   private:
      bool PlayerLeft;

   public:
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      CElementEnemyProjectileGrape(SRect2f nArea);

      virtual void printInformation();
};
#endif
