#include "CPlayerAbilityHeadbutt.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityHeadbutt::HEADBUTT_CHARGE_DURATION = 0.5f;
float const CPlayerAbilityHeadbutt::HEADBUTT_FIRING_DURATION = 0.3f;
int const CPlayerAbilityHeadbutt::HEADBUTT_CHARGE_PARTICLE_COUNT =  700;
int const CPlayerAbilityHeadbutt::HEADBUTT_FIRING_PARTICLE_COUNT = 1000;

#include "CGameLevel.h"
void CPlayerAbilityHeadbutt::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   TemporaryTimeVariable += time;
   if(HeadbuttState == FIRING) {
   }
   return;
}

void CPlayerAbilityHeadbutt::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(ParticleEngine) {
      //Update the particles
      if(HeadbuttState == CHARGED) {
         ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter().X, Player.getArea().Position.Y, 0.0f));
      }
      else
         ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter(), 0.0f));
      ParticleEngine->step(time);
   }
   if(HeadbuttState == FIRING)
      Player.View->setVisible(true);
}

void CPlayerAbilityHeadbutt::inOnCollision(const SCollisionEvent& Event) {
   if(HeadbuttState == FIRING)
      if(Event.Other->getGameplayElement())
         Event.Other->getGameplayElement()->reactToAbility(Type);
}

CPlayerAbilityHeadbutt::CPlayerAbilityHeadbutt(CElementPlayer & p) : CPlayerAbility(p, Abilities::HEADBUTT), HeadbuttState(CHARGING), TemporaryTimeVariable(0.0f) {
   EnergyUsed = 5;

   if (Player.Stats.Energy >= EnergyUsed)
      ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), HEADBUTT_CHARGE_PARTICLE_COUNT, HEADBUTT_CHARGE_DURATION, LASER_CHARGING_PARTICLE);
   else
      Dead = true;
}

void CPlayerAbilityHeadbutt::checkKey(bool keyDown) {
   if(Dead)
      return;
   if(HeadbuttState == CHARGING) {
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
      if(TemporaryTimeVariable >= HEADBUTT_CHARGE_DURATION) {
         Player.Stats.Energy -= EnergyUsed;
         Player.AllowMovement = true;
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), 20, -1, LASER_CHARGED_PARTICLE);
         HeadbuttState = CHARGED;
         return;
      }
   }
   else if(HeadbuttState == CHARGED) {
      if(!keyDown) {
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         Player.AllowMovement = false;
         /*
         ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), HEADBUTT_FIRING_PARTICLE_COUNT, HEADBUTT_FIRING_DURATION, LASER_FIRING_PARTICLE);
         ParticleEngine->setLookRight(Player.Direction == CElementPlayer::Right);
         */
         ParticleEngine = NULL;
         HeadbuttState = FIRING;
         TemporaryTimeVariable = 0;
         tempGrav = ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().AirStandingFriction;
         printf("Tempgrav: %0.2f\n", ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().AirStandingFriction);
         ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().AirStandingFriction = ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().GroundStandingFriction;
         ((CCollisionActor *)Player.getPhysicsEngineObject())->addImpulse(SVector2f((Player.Direction == CElementPlayer::Right ? 1 : -1) * 65.0f, 0.0f));
         ((CCollisionActor *)Player.getPhysicsEngineObject())->setCollisionMask(((CCollisionActor *)Player.getPhysicsEngineObject())->getCollisionMask() & ~INTERACTOR_ACTORS);
         ((CCollisionActor *)Player.getPhysicsEngineObject())->setDetectionMask(((CCollisionActor *)Player.getPhysicsEngineObject())->getDetectionMask() | INTERACTOR_ACTORS);
         Player.View->setTilt(60.0f);
         return;
      }
   }
   else if(HeadbuttState == FIRING) {
      Player.Recovering = 5.0f;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.AllowMovement = false;
      if(TemporaryTimeVariable >= HEADBUTT_FIRING_DURATION) {
         ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().AirStandingFriction = tempGrav;
         printf("ASF: %0.2f\n", ((CCollisionActor *)Player.getPhysicsEngineObject())->getAttributes().AirStandingFriction);
         /*
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         */
         Dead = true;
         HeadbuttState = FIRED;
         //((CCollisionActor *)Player.getPhysicsEngineObject())->addImpulse(SVector2f((Player.Direction == CElementPlayer::Right ? 1.0f : -1.0f)*15.0f, 0.0f));
         Player.AllowMovement = true;
         Player.View->setVisible(true);
         Player.View->setHurt(false);
         Player.Recovering = 0.0f;
         ((CCollisionActor *)Player.getPhysicsEngineObject())->setCollisionMask(((CCollisionActor *)Player.getPhysicsEngineObject())->getCollisionMask() | INTERACTOR_ACTORS);
         ((CCollisionActor *)Player.getPhysicsEngineObject())->setDetectionMask(((CCollisionActor *)Player.getPhysicsEngineObject())->getDetectionMask() & ~INTERACTOR_ACTORS);
         Player.View->setTilt(0.0f);
      }
      else {
         Player.View->setTilt((HEADBUTT_FIRING_DURATION - TemporaryTimeVariable)/HEADBUTT_FIRING_DURATION*60.0f);
      }
   }
   //This is NEVER RUNNING, FYI
   else if(HeadbuttState == FIRED) {
      Player.AllowMovement = true;
   }
}
