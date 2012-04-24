#include "CElementEnemyProjectile.h"
#include "CGameLevel.h"

#include "CElementEnemyProjectileGrape.h"
#include "CElementEnemyProjectileKiwi.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectile::CElementEnemyProjectile(SRect2 nArea, Enemies::EEnemyType type)
: CElementEnemy(nArea, type) {
}

//Re-write me to die on impact.
void CElementEnemyProjectile::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
         
      if(Level.getPlayer().decrementHealth()) {
         if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
            PlayerActor->setImpulse(SVector2(7.f, 2.8f), 0.1f);
         else
            PlayerActor->setImpulse(SVector2(-7.f, 2.8f), 0.1f);

         Level.getPlayer().setShaking(1.0f, 3.0f);
      }
   }
}

/*If you want to be able to kill projectiles by jumping on them, implement this code in OnCollision:
 
  if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f) {
         Level.removeEnemy(this);
         removeFromEngines();
         Dead = true;
         
         PlayerActor->setImpulse(SVector2(0.0f, 3.0f), 0.01f);
         Level.getPlayer().setShaking(0.4f, 3.0f);
      }
      else {
         if(Level.getPlayer().decrementHealth()) {
            if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
               PlayerActor->setImpulse(SVector2(7.f, 2.8f), 0.1f);
            else
               PlayerActor->setImpulse(SVector2(-7.f, 2.8f), 0.1f);
            Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
*/

void CElementEnemyProjectile::updatePhysicsEngineObject(float time) {
   return;
}

void CElementEnemyProjectile::updateSceneObject(float time) {
   return;
}

void CElementEnemyProjectile::printInformation() {
   printf("CElementEnemyProjectile; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Type);
}
