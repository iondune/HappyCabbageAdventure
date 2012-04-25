#ifndef CPLAYERABILITYBLINK___H
#define CPLAYERABILITYBLINK___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityBlink : public CPlayerAbility {
   //CParticleEngine *ParticleEngine;
   static float const BLINK_DURATION = 0.01f;
   //static int const BLINK_PARTICLE_COUNT = 150;

   public:
      void inUpdatePhysicsEngineObject(float);
      void inUpdateSceneObject(float);
      //void inOnCollision(CCollideable *);
      CPlayerAbilityBlink(CElementPlayer &);
};

#endif
