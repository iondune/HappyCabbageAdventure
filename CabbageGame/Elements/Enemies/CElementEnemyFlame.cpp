#include "CElementEnemyFlame.h"
#include "CGameLevel.h"

CElementEnemyFlame::CElementEnemyFlame(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::FLAME) {

}

void CElementEnemyFlame::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->setGravityEnabled(false);

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyFlame::setupSceneObject() {
   particleEngine = new CParticleEngine(SVector3f(0.0f), 50, -1, FLAME_PARTICLE, Level.isNight());
   //particleEngine = new CParticleEngine(SVector3(Area.Position.X, Area.Position.Y, 0), 100, -1, FLAME_PARTICLE, Level.isNight());
   SceneObject = (CMeshSceneObject*) particleEngine->getSceneObject();
   SceneObject->setTranslation(SVector3f(Area.Position.X, Area.Position.Y, 0));
}

void CElementEnemyFlame::OnCollision(const SCollisionEvent& Event) {
   static float const flameJumpFactor = 8.0f;
   if(!Dead) {
      if(Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
         if(Level.getPlayer().decrementHealth()) {
            CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
            if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f) {
               PlayerActor->addImpulse(SVector2f(0.0f, flameJumpFactor));
            }
         }
      }
      else {
         //We can make enemies jump when they touch fire here too, once we have a pointer to the CElementEnemy*.
         ((CCollisionActor *)Event.Other)->addImpulse(SVector2f(0.0f, flameJumpFactor));
      }
   }
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyFlame::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyFlame::updateSceneObject(float time) {
   particleEngine->step(time);
}

void CElementEnemyFlame::printInformation() {
   printf("CElementEnemyFlame; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyFlame::reactToAbility(Abilities::EAbilityType Ability) {
   return;
}
