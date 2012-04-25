#ifndef CPLAYERABILITYLASER___H
#define CPLAYERABILITYLASER___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityLaser : public CPlayerAbility {
   public:
      enum ELaserState {
         CHARGING		= 0,
         CHARGED		= 1,
         FIRING		= 2,
         FIRED			= 3
      };

   private:
      static float const LASER_CHARGE_DURATION;
      static float const LASER_FIRING_DURATION;
      static int const LASER_CHARGE_PARTICLE_COUNT;
      static int const LASER_FIRING_PARTICLE_COUNT;

      CParticleEngine *ParticleEngine;
      ELaserState LaserState;
      float TemporaryTimeVariable;
      SRect2 TemporaryArea;

   public:
      void inUpdatePhysicsEngineObject(float);
      void inUpdateSceneObject(float);
      void inOnCollision(CCollideable *Object);
      void checkKey(bool keyDown);
      //void inOnCollision(CCollideable *);
      CPlayerAbilityLaser(CElementPlayer &);
};

#endif
