#include "CPlayerAbility.h"
#include "CElementPlayer.h"

CPlayerAbility::CPlayerAbility(CElementPlayer & p, Abilities::EAbilityType a) : Player(p), ElapsedTime(0.0f), Dead(false), Type(a) {

}

//Default behavior: call React on enemies that collide with the cabbage while the ability is running.
void CPlayerAbility::inOnCollision(CCollideable *Object) {
   Object->getElement().reactToAbility(Type);
}

void CPlayerAbility::updateTime(float time) {
   ElapsedTime += time;
}
