#ifndef ____CELEMENTENEMYPROJECTILE_HEADER_
#define ____CELEMENTENEMYPROJECTILE_HEADER_
#include "../CGameplayElement.h"
#include "CElementPlayer.h"
#include "CElementEnemy.h"

namespace Projectile {
   enum EDirection {
      LEFT, RIGHT
   };
}

class CElementEnemyProjectile : public CElementEnemy {
   public:

   protected:
      Projectile::EDirection Direction;

   public:
      virtual void OnCollision(CCollideable *Object);

      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
      void reactToAbility(Abilities::EAbilityType Type);

      void setDirection (Projectile::EDirection direction);

      CElementEnemyProjectile(SRect2f nArea, Enemies::EEnemyType type);

      virtual void printInformation();
};
#endif
