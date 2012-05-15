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
   ParticleEngine->setCenterPos(SVector3(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);
}

/*
void CPlayerAbilityDash::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityDash::CPlayerAbilityDash(CElementPlayer & p) : CPlayerAbility(p, Abilities::DASH) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), DASH_PARTICLE_COUNT, -1, DUST_PARTICLE);
   ParticleEngine->UsePhysics(&Player.Level.getPhysicsEngine());
   ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 7.5f;

   EnergyTime = .5f;
}

void CPlayerAbilityDash::checkKey(bool keyDown) {
   if(!keyDown) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 3.5f;
      Dead = true;
      return;
   }
}
