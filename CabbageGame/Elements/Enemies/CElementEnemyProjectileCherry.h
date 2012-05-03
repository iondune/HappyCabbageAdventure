#ifndef ____CELEMENTENEMYPROJECTILECHERRY_HEADER_
#define ____CELEMENTENEMYPROJECTILECHERRY_HEADER_

#include "CElementEnemyProjectile.h"

class CElementEnemyProjectileCherry : public CElementEnemyProjectile {
   private:
      float XDir;
      bool SetImpulse;

   public:
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      CElementEnemyProjectileCherry(SRect2 nArea, float xDirection);

      virtual void printInformation();
};
#endif
