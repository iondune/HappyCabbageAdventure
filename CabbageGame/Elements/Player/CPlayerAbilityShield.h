#ifndef CPLAYERABILITYSHIELD___H
#define CPLAYERABILITYSHIELD___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityShield : public CPlayerAbility {
   CParticleEngine *ParticleEngine;

   public:
      void inUpdatePhysicsEngine(float);
      void inUpdateSceneObject(float);
      void inOnCollision(CCollideable *);
      CPlayerAbilityShield(CElementPlayer &);
};

#endif
