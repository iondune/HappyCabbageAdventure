#ifndef CPLAYERABILITYSHIELD___H
#define CPLAYERABILITYSHIELD___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityShield : public CPlayerAbility {
   CParticleEngine *ParticleEngine;
   static float const LEAF_SHIELD_DURATION = 6.0f;
   static int const LEAF_SHIELD_PARTICLE_COUNT = 150;

   public:
      void inUpdatePhysicsEngineObject(float);
      void inUpdateSceneObject(float);
      void inOnCollision(CCollideable *);
      CPlayerAbilityShield(CElementPlayer &);
};

#endif
