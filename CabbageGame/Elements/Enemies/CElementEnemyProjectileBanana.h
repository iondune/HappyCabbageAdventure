#ifndef ____CELEMENTENEMYPROJECTILEBanana_HEADER_
#define ____CELEMENTENEMYPROJECTILEBanana_HEADER_

#include "CElementEnemyProjectile.h"

class CElementEnemyProjectileBanana : public CElementEnemyProjectile {
   private:
      bool PlayerLeft;
      SVector3 Rotation;
      float SinValue, CosValue;

   public:
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      CElementEnemyProjectileBanana(SRect2f nArea);

      virtual void printInformation();
};
#endif
