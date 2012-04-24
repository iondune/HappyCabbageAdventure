#include "CElementEnemyProjectileGrape.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileGrape::CElementEnemyProjectileGrape(SRect2 nArea)
: CElementEnemyProjectile(nArea, Enemies::GRAPE_PROJECTILE) {
}

void CElementEnemyProjectileGrape::updatePhysicsEngineObject(float time) {
   return;
}

void CElementEnemyProjectileGrape::updateSceneObject(float time) {
   return;
}

void CElementEnemyProjectileGrape::printInformation() {
   printf("CElementEnemyProjectileGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
