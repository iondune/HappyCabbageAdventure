#include "CPlayerAbilityLaser.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityLaser::LASER_CHARGE_DURATION;
float const CPlayerAbilityLaser::LASER_FIRING_DURATION;
int const CPlayerAbilityLaser::LASER_CHARGE_PARTICLE_COUNT;
int const CPlayerAbilityLaser::LASER_FIRING_PARTICLE_COUNT;

void CPlayerAbilityLaser::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   TemporaryTimeVariable += time;
   return;
}

void CPlayerAbilityLaser::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(ParticleEngine) {
      //Update the particles
      ParticleEngine->setCenterPos(SVector3(Player.getArea().getCenter(), 0.0f));
      ParticleEngine->step(time);
   }
   if(LaserState == FIRING)
      Player.View->setVisible(true);
}

void CPlayerAbilityLaser::inOnCollision(CCollideable * collider) {
   return;
}

CPlayerAbilityLaser::CPlayerAbilityLaser(CElementPlayer & p) : CPlayerAbility(p, Abilities::LASER), LaserState(CHARGING), TemporaryTimeVariable(0.0f) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), LASER_CHARGE_PARTICLE_COUNT, LASER_CHARGE_DURATION, LASER_CHARGING_PARTICLE);
}

void CPlayerAbilityLaser::checkKey(bool keyDown) {
   if(Dead)
      return;
   if(LaserState == CHARGING) {
      Player.AllowMovement = false;
      if(!keyDown) {
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Dead = true;
         return;
      }
      if(TemporaryTimeVariable >= LASER_CHARGE_DURATION) {
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         ParticleEngine = NULL;
         LaserState = CHARGED;
         return;
      }
   }
   else if(LaserState == CHARGED) {
      if(!keyDown) {
         Player.AllowMovement = false;
         ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), LASER_FIRING_PARTICLE_COUNT, LASER_FIRING_DURATION, LASER_FIRING_PARTICLE);
         ParticleEngine->setLookRight(Player.Direction == CElementPlayer::Right);
         LaserState = FIRING;
         TemporaryTimeVariable = 0;
         TemporaryArea = Player.getArea();
         Player.setShaking(1.2f, 0.3f);
         return;
      }
   }
   else if(LaserState == FIRING) {
      Player.Recovering = 5.0f;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.getPhysicsEngineObject()->setArea(TemporaryArea);
      ((CCollisionActor *)Player.getPhysicsEngineObject())->setVelocity(SVector2(0.0f));
      ((CCollisionActor *)Player.getPhysicsEngineObject())->setFallAcceleration(0.0f);
      Player.AllowMovement = false;
      if(TemporaryTimeVariable >= LASER_FIRING_DURATION) {
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Dead = true;
         LaserState = FIRED;
         ((CCollisionActor *)Player.getPhysicsEngineObject())->setImpulse(SVector2((Player.Direction == CElementPlayer::Right ? -1.0f : 1.0f)*15.0f, 0.0f), 0.1f);
         Player.AllowMovement = true;

         Player.View->setVisible(true);
         Player.View->setHurt(false);
         Player.Recovering = 0.0f;
      } 
   }
   else if(LaserState == FIRED) {
      Player.AllowMovement = true;
   }
}
