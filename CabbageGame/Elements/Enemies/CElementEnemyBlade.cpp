#include "CElementEnemyBlade.h"
#include "CGameLevel.h"

CElementEnemyBlade::CElementEnemyBlade(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::BLADE), BladeRotate(0.0f) {

}

void CElementEnemyBlade::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->setGravityEnabled(false);

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyBlade::setupSceneObject() {
   /* Set up the renderable */
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/newspinningknife.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(0.5));
      mesh->centerMeshByExtents(SVector3f(0.11649f, 0, 0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter().X, PhysicsEngineObject->getArea().getCenter().Y, 0));
   SceneObject->setScale(SVector3f(2, 2, 2));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyBlade::OnCollision(const SCollisionEvent& Event) {
   static float const bladeJumpFactor = 6.0f;
   if(!Dead) {
      if(Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
         if(Level.getPlayer().decrementHealth()) {
            CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
            if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f) {
               PlayerActor->addImpulse(SVector2f(0.0f, bladeJumpFactor));
            }
         }
      }
      else {
         //We can make enemies jump when they touch fire here too, once we have a pointer to the CElementEnemy*.
         ((CCollisionActor *)Event.Other)->addImpulse(SVector2f(0.0f, bladeJumpFactor));
      }
   }
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyBlade::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   return; //Doesn't move in physics engine
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyBlade::updateSceneObject(float time) {
   BladeRotate -= 300.0f * time;
   SceneObject->setRotation(SVector3f(-90, 0, BladeRotate));
}

void CElementEnemyBlade::printInformation() {
   printf("CElementEnemyBlade; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyBlade::reactToAbility(Abilities::EAbilityType Ability) {
   return;
}
