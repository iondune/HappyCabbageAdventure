#ifndef CELEMENTENEMYFLAME_H_
#define CELEMENTENEMYFLAME_H_

#include "CElementEnemy.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "../CabbageParticles/CPFlame.h"
#include "CGameplayManager.h"

class CElementEnemyFlame : public CElementEnemy
{
   public:
   CParticleEngine *particleEngine;

   public:
      CElementEnemyFlame(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable*);
      void reactToAbility(Abilities::EAbilityType Ability);

      virtual void printInformation();
};

#endif
