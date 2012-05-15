#include "CPlayerAbilityBlink.h"
#include "CElementPlayer.h"
#include "CPlayerView.h"
float const CPlayerAbilityBlink::BLINK_DURATION = 0.1f;
//int const CPlayerAbilityBlink::BLINK_PARTICLE_COUNT;

void CPlayerAbilityBlink::inUpdatePhysicsEngineObject(float time) {
   if(Dead)
      return;
   //To make the cabbage impervious to damage
   Player.Recovering = 5.0f;
   Player.View->setVisible(true);
   Player.View->setHurt(false);
   return;
}

void CPlayerAbilityBlink::inUpdateSceneObject(float time) {
   if(Dead)
      return;
   if(ElapsedTime >= BLINK_DURATION) {
      printf("Here\n");
      Player.View->setVisible(true);
      Player.View->setHurt(false);
      Player.Recovering = 0.0f;
      Dead = true;
      return;
   }
   //Update the particles
   Player.View->setVisible(true);
}

/*
void CPlayerAbilityBlink::inOnCollision(CCollideable * collider) {
   return;
}
*/

CPlayerAbilityBlink::CPlayerAbilityBlink(CElementPlayer & p) : CPlayerAbility(p, Abilities::BLINK) {
   Player.getPhysicsEngineObject()->setArea(SRect2f(Player.getArea().Position + SVector2f(3.0f, 0.0f), Player.getArea().Size));
}
