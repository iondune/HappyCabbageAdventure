#include "CPlayerAbilityDash.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
int const CPlayerAbilityDash::DASH_PARTICLE_COUNT = 70;

void CPlayerAbilityDash::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   //To make the cabbage impervious to damage
   Player.Recovering = 5.0f;
   Player.View->setVisible(true);
   Player.View->setHurt(false);
   return;
}

void CPlayerAbilityDash::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(Player.Stats.Energy <= 0) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Dead = true;
      return;
   }
   //Update the particles
   ParticleEngine->setLookRight(Player.Direction == CElementPlayer::Right);
   ParticleEngine->setCenterPos(SVector3(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);

   EnergyTime -= time;

   if (EnergyTime <= 0.0f) {
      Player.Stats.Energy-= 1;
      EnergyTime = 1.0f;
   }
}

/*
void CPlayerAbilityDash::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityDash::CPlayerAbilityDash(CElementPlayer & p) : CPlayerAbility(p, Abilities::SHIELD) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), DASH_PARTICLE_COUNT, -1, DUST_PARTICLE);
   ParticleEngine->UsePhysics(&Player.Level.getPhysicsEngine());
   ((CCollisionActor*)Player.getPhysicsEngineObject())->getAttributes().MaxWalk = 7.5f;

   if (Player.Stats.Energy <= 0) {
      Dead = true;
   }

   EnergyTime = .05f;
}

void CPlayerAbilityDash::checkKey(bool keyDown) {
   if(!keyDown || Player.Stats.Energy <= 0) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Dead = true;
      return;
   }
}
