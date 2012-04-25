#ifndef ____CELEMENTENEMYPROJECTILE_HEADER_
#define ____CELEMENTENEMYPROJECTILE_HEADER_
#include "../CGameplayElement.h"
#include "CElementPlayer.h"
#include "CElementEnemy.h"

class CElementEnemyProjectile : public CElementEnemy {
   public:

   protected:

   public:
      virtual void OnCollision(CCollideable *Object);

      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      CElementEnemyProjectile(SRect2 nArea, Enemies::EEnemyType type);

      virtual void printInformation();
};
#endif
