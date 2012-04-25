#include "CPlayerAbilityShield.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityShield::LEAF_SHIELD_DURATION = 60.0f;
int const CPlayerAbilityShield::LEAF_SHIELD_PARTICLE_COUNT = 150;

void CPlayerAbilityShield::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   //To make the cabbage impervious to damage
   Player.Recovering = 5.0f;
   Player.View->setVisible(true);
   Player.View->setHurt(false);
   return;
}

void CPlayerAbilityShield::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(ElapsedTime >= LEAF_SHIELD_DURATION) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.Recovering = 0.0f;
      Dead = true;
      return;
   }
   //Update the particles
   ParticleEngine->setCenterPos(SVector3(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);
   Player.View->setVisible(true);
}

/*
void CPlayerAbilityShield::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityShield::CPlayerAbilityShield(CElementPlayer & p) : CPlayerAbility(p, Abilities::SHIELD) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), LEAF_SHIELD_PARTICLE_COUNT, -1, LEAF_PARTICLE);
}

void CPlayerAbilityShield::checkKey(bool keyDown) {
   if(!keyDown) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.Recovering = 0.0f;
      Dead = true;
      return;
   }
}
