#include "CPlayerAbilityShield.h"
#include "CElementPlayer.h"

void CPlayerAbilityShield::inUpdatePhysicsEngine(float time) {
   //To make the cabbage impervious to damage
   Recovering = 5.0f;
   return;
}

void CPlayerAbilityShield::inUpdateSceneObject(float time) {
   //Update the particles
   ParticleEngine->setCenterPos(SVector3(Player.getArea().getCenter(), 0.0f));
   ParticleEngine->step(time);
}

void CPlayerAbilityShield::inOnCollision(CCollideable *Object) {
   if(Object->
}

CPlayerAbilityShield::CPlayerAbilityShield(CElementPlayer & p) : CPlayerAbility(p) {
   ParticleEngine = new CParticleEngine(SVector3(0, 1, 0), 150, 6, LEAF_PARTICLE);
}
