#include "CElementEnemyProjectileKiwi.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileKiwi::CElementEnemyProjectileKiwi(SRect2 nArea)
: CElementEnemyProjectile(nArea, Enemies::KIWI_PROJECTILE) {
   printf("New kiwi projectile\n");
}

void CElementEnemyProjectileKiwi::updatePhysicsEngineObject(float time) {
   return;
}

void CElementEnemyProjectileKiwi::updateSceneObject(float time) {
   return;
}

void CElementEnemyProjectileKiwi::printInformation() {
   printf("CElementEnemyProjectileKiwi; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
