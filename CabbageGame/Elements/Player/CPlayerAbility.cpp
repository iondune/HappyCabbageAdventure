#include "CPlayerAbility.h"
#include "CElementPlayer.h"

CPlayerAbility::CPlayerAbility(CElementPlayer & p, Abilities::EAbilityType a) : Player(p), ElapsedTime(0.0f), Dead(false), Type(a) {

}

//Default behavior: call React on enemies that collide with the cabbage while the ability is running.
void CPlayerAbility::inOnCollision(const SCollisionEvent& Event) {
   if(Event.Other->getGameplayElement())
      Event.Other->getGameplayElement()->reactToAbility(Type);
}

void CPlayerAbility::updateTime(float time) {
   ElapsedTime += time;
}

bool CPlayerAbility::isDead() {
   return Dead;
}

Abilities::EAbilityType CPlayerAbility::getType() {
   return Type;
}

void CPlayerAbility::checkKey(bool keyDown) {
   
}
