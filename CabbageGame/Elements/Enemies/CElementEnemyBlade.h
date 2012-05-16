#ifndef CELEMENTENEMYBLADE_H_
#define CELEMENTENEMYBLADE_H_

#include "CElementEnemy.h"

class CElementEnemyBlade : public CElementEnemy
{
   private:
      float BladeRotate;

   public:
      CElementEnemyBlade(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
      
      virtual void OnCollision(const SCollisionEvent& Event);
      void reactToAbility(Abilities::EAbilityType Ability);

      virtual void printInformation();
};

#endif
