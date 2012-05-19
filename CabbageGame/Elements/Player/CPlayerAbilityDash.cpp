#include "CPlayerAbilityDash.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
int const CPlayerAbilityDash::DASH_PARTICLE_COUNT = 70;

void CPlayerAbilityDash::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   return;
}

void CPlayerAbilityDash::inUpdateSceneObject(float time) {
   if(Dead)
      return;

   //Update the particles
   ParticleEngine->setLookRight(Player.Direction == CElementPlayer::Right);
   ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);
}

/*
void CPlayerAbilityDash::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityDash::CPlayerAbilityDash(CElementPlayer & p, bool doGodmode) : CPlayerAbility(p, Abilities::DASH), Godmode(doGodmode) {
   ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), DASH_PARTICLE_COUNT, -1, DUST_PARTICLE);
   ParticleEngine->UsePhysics(&Player.Level.getPhysicsEngine(), Player.Level.getEnv());
   if(!Godmode && Player.Level.getEnv() != Env::WATER)
      ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 7.5f;
   else if (!Godmode && Player.Level.getEnv() == Env::WATER)
      ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 3.75f;
   else {
      ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 34.5f;
   }

   EnergyTime = .5f;
}

void CPlayerAbilityDash::checkKey(bool keyDown) {
   if(!keyDown) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      if(Player.Level.getEnv() != Env::WATER)
         ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 3.5f;
      else
         ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 1.75f;
      Dead = true;
      if(Godmode) {
      }
      return;
   }
}
