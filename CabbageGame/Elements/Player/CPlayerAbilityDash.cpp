#include "CPlayerAbilityDash.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityDash::DASH_DURATION = 60.0f;
int const CPlayerAbilityDash::DASH_PARTICLE_COUNT = 150;

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
   if(ElapsedTime >= DASH_DURATION || Player.Stats.Energy <= 0) {
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

   EnergyTime -= time;

   if (EnergyTime <= 0.0f) {
      Player.Stats.Energy-= 1;
      EnergyTime = .05f;
   }
}

/*
void CPlayerAbilityDash::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityDash::CPlayerAbilityDash(CElementPlayer & p) : CPlayerAbility(p, Abilities::SHIELD) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), DASH_PARTICLE_COUNT, -1, DUST_PARTICLE);

   if (Player.Stats.Energy <= 0) {
      Dead = true;
   }

   EnergyTime = .05f;
}

void CPlayerAbilityDash::checkKey(bool keyDown) {
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
