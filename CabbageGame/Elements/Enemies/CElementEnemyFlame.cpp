#include "CElementEnemyFlame.h"
#include "CGameLevel.h"

CElementEnemyFlame::CElementEnemyFlame(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::FLAME) {

}

void CElementEnemyFlame::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Makes them immune to gravity
   PhysicsEngineObject->setControlFall(false);
   PhysicsEngineObject->setFallAcceleration(0.0f);
   //PhysicsEngineObject->setArea(SRect2(SVector2(Area.Position.X, Area.Position.Y-1.0f), Area.Size));
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_FLAME;
}

void CElementEnemyFlame::setupSceneObject() {
   particleEngine = new CParticleEngine(SVector3(Area.Position.X, Area.Position.Y, 0), 100, -1, FLAME_PARTICLE);
}

void CElementEnemyFlame::OnCollision(CCollideable *Object) {
   static float const flameJumpFactor = 6.0f;
   if(!Dead) {
      if(Object == Level.getPlayer().getPhysicsEngineObject()) {
         if(Level.getPlayer().decrementHealth()) {
            CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
            if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f) {
               PlayerActor->setImpulse(SVector2(0.0f, flameJumpFactor), 0.01f);
            }
         }
      }
      else {
         //We can make enemies jump when they touch fire here too, once we have a pointer to the CElementEnemy*.
         ((CCollisionActor *)Object)->setImpulse(SVector2(0.0f, flameJumpFactor), 0.01f);
      }
   }
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyFlame::updatePhysicsEngineObject(float time) {
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyFlame::updateSceneObject(float time) {
   particleEngine->step(time);
}

void CElementEnemyFlame::printInformation() {
   printf("CElementEnemyFlame; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
