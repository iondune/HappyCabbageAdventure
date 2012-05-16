#include "CPlayerAbilityLaser.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityLaser::LASER_CHARGE_DURATION = 1.0f;
float const CPlayerAbilityLaser::LASER_FIRING_DURATION = 1.2f;
int const CPlayerAbilityLaser::LASER_CHARGE_PARTICLE_COUNT =  700;
int const CPlayerAbilityLaser::LASER_FIRING_PARTICLE_COUNT = 1000;

#include "CGameLevel.h"
void CPlayerAbilityLaser::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   TemporaryTimeVariable += time;
   if(LaserState == FIRING) {
      SRect2f FiringRange = Player.Area;
      if(Player.Direction == CElementPlayer::Right) {
         FiringRange.Size.X += 5.0f;
      }
      else {
         FiringRange.Position.X -= 5.0f;
         FiringRange.Size.X += 5.0f;
      }
      std::vector<CCollideable*> InRange = Player.Level.getPhysicsEngine().getAllInBound(FiringRange);
      for(unsigned int i = 0; i < InRange.size(); i++) {
         if(InRange[i]->getGameplayElement())
            InRange[i]->getGameplayElement()->reactToAbility(Abilities::LASER);
      }
   }
   return;
}

void CPlayerAbilityLaser::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(ParticleEngine) {
      //Update the particles
      if(LaserState == CHARGED) {
         ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter().X, Player.getArea().Position.Y, 0.0f));
      }
      else
         ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter(), 0.0f));
      ParticleEngine->step(time);
   }
   if(LaserState == FIRING)
      Player.View->setVisible(true);
}

void CPlayerAbilityLaser::inOnCollision(const SCollisionEvent& Event) {
   return;
}

CPlayerAbilityLaser::CPlayerAbilityLaser(CElementPlayer & p) : CPlayerAbility(p, Abilities::LASER), LaserState(CHARGING), TemporaryTimeVariable(0.0f) {
   EnergyUsed = 33;

   if (Player.Stats.Energy >= EnergyUsed)
      ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), LASER_CHARGE_PARTICLE_COUNT, LASER_CHARGE_DURATION, LASER_CHARGING_PARTICLE);
   else
      Dead = true;
}

void CPlayerAbilityLaser::checkKey(bool keyDown) {
   if(Dead)
      return;
   if(LaserState == CHARGING) {
      if (Player.Stats.Energy < EnergyUsed) {
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Dead = true;
         return;
      }
      Player.AllowMovement = false;
      if(!keyDown) {
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Dead = true;
         return;
      }
      if(TemporaryTimeVariable >= LASER_CHARGE_DURATION) {
         Player.Stats.Energy -= EnergyUsed;
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), 20, -1, LASER_CHARGED_PARTICLE);
         LaserState = CHARGED;
         return;
      }
   }
   else if(LaserState == CHARGED) {
      if(!keyDown) {
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Player.AllowMovement = false;
         ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), LASER_FIRING_PARTICLE_COUNT, LASER_FIRING_DURATION, LASER_FIRING_PARTICLE);
         ParticleEngine->setLookRight(Player.Direction == CElementPlayer::Right);
         LaserState = FIRING;
         TemporaryTimeVariable = 0;
         TemporaryArea = Player.getArea();
         Player.setShaking(1.2f, 0.3f);
         return;
      }
   }
   else if(LaserState == FIRING) {
      Mix_PlayChannel(-1, Player.fireLaser, 0);
      Player.Recovering = 5.0f;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.getPhysicsEngineObject()->setArea(TemporaryArea);
      ((CCollisionActor *)Player.getPhysicsEngineObject())->setVelocity(SVector2f(0.0f));
      ((CCollisionActor *)Player.getPhysicsEngineObject())->setFallAcceleration(0.0f);
      Player.AllowMovement = false;
      if(TemporaryTimeVariable >= LASER_FIRING_DURATION) {
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Dead = true;
         LaserState = FIRED;
         ((CCollisionActor *)Player.getPhysicsEngineObject())->addImpulse(SVector2f((Player.Direction == CElementPlayer::Right ? -1.0f : 1.0f)*15.0f, 0.0f), 0.001f);
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
