#ifndef CPLAYERABILITYHEADBUTT___H
#define CPLAYERABILITYHEADBUTT___H

#include "CPlayerAbility.h"
#include "CParticleEngine.h"

class CPlayerAbilityHeadbutt : public CPlayerAbility {
   public:
      enum EHeadbuttState {
         CHARGING      = 0,
         CHARGED      = 1,
         FIRING      = 2,
         FIRED         = 3
      };

   private:
      static float const HEADBUTT_CHARGE_DURATION;
      static float const HEADBUTT_FIRING_DURATION;
      static int const HEADBUTT_CHARGE_PARTICLE_COUNT;
      static int const HEADBUTT_FIRING_PARTICLE_COUNT;
      int EnergyUsed;
      CollisionReal tempGrav;

      CParticleEngine *ParticleEngine;
      EHeadbuttState HeadbuttState;
      float TemporaryTimeVariable;
      SRect2f TemporaryArea;

   public:
      void inUpdatePhysicsEngineObject(float);
      void inUpdateSceneObject(float);
      void inOnCollision(SCollisionEvent const &);
      void checkKey(bool keyDown);
      //void inOnCollision(CCollideable *);
      CPlayerAbilityHeadbutt(CElementPlayer &);
};

#endif
