#include "CElementEnemyProjectile.h"
#include "CGameLevel.h"

#include "CElementEnemyProjectileGrape.h"
#include "CElementEnemyProjectileKiwi.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectile::CElementEnemyProjectile(SRect2f nArea, Enemies::EEnemyType type)
: CElementEnemy(nArea, type) {
   Mix_PlayChannel(-1, Level.projectile, 0);
}

//Re-write me to die on impact.
void CElementEnemyProjectile::OnCollision(const SCollisionEvent& Event) {
   if (!Dead) {
      removeFromGame();
      Dead = true;

      if (Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
         CCollisionActor *PlayerActor = ((CCollisionActor *)Level.getPlayer().getPhysicsEngineObject());

         if (Level.getPlayer().decrementHealth()) {

           if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
              PlayerActor->addImpulse(SVector2f(4.f, 2.f), 0.1f);
           
           else
              PlayerActor->addImpulse(SVector2f(-4.f, 2.f), 0.1f);
         
           Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }  
   }  
}

void CElementEnemyProjectile::updatePhysicsEngineObject(float time) {
   return;
}

void CElementEnemyProjectile::updateSceneObject(float time) {
   return;
}

void CElementEnemyProjectile::printInformation() {
   printf("CElementEnemyProjectile; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Type);
}

void CElementEnemyProjectile::reactToAbility(Abilities::EAbilityType Type) {
   switch(Type) {
   case Abilities::SHIELD:
   case Abilities::LASER:
      removeFromGame();
      break;
   }
}

void CElementEnemyProjectile::setDirection(Projectile::EDirection direction) {
   Direction = direction;
}
