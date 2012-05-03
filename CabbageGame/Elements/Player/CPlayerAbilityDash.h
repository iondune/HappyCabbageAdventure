#ifndef CPLAYERABILITYDASH___H
#define CPLAYERABILITYDASH___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityDash : public CPlayerAbility {
   CParticleEngine *ParticleEngine;
   static int const DASH_PARTICLE_COUNT;

   private:
      float EnergyTime;

   public:
      void inUpdatePhysicsEngineObject(float);
      void inUpdateSceneObject(float);
      void checkKey(bool keyDown);
      //void inOnCollision(CCollideable *);
      CPlayerAbilityDash(CElementPlayer &);
};

#endif
