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
   if(ElapsedTime >= LEAF_SHIELD_DURATION || Player.Stats.Energy <= 0) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.Recovering = 0.0f;
      Dead = true;
      return;
   }
   //Update the particles
   ParticleEngine->setCenterPos(SVector3f(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);
   Player.View->setVisible(true);

   EnergyTime -= time;

   if (EnergyTime <= 0.0f) {
      Player.Stats.Energy-= 1;
      EnergyTime = .05f;
   }
}

/*
void CPlayerAbilityShield::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityShield::CPlayerAbilityShield(CElementPlayer & p) : CPlayerAbility(p, Abilities::SHIELD) {
   ParticleEngine = new CParticleEngine(SVector3f(0, 1, 0), LEAF_SHIELD_PARTICLE_COUNT, -1, LEAF_PARTICLE);

   if (Player.Stats.Energy <= 0) {
      Dead = true;
   }

   EnergyTime = .05f;
}

void CPlayerAbilityShield::checkKey(bool keyDown) {
   if(!keyDown || Player.Stats.Energy <= 0) {
      ParticleEngine->deconstruct();
      delete ParticleEngine;
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.Recovering = 0.0f;
      Dead = true;
      return;
   }
}
