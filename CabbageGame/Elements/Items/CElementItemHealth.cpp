#include "CElementItemHealth.h"
#include "CGameLevel.h"

CElementItemHealth::CElementItemHealth(SRect2f nArea) :
   CElementItem(nArea, Items::HEALTH) {

}

void CElementItemHealth::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/healthboost.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter(), 0));
   SceneObject->setScale(SVector3f(.8f));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemHealth::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject() && !Level.getPlayer().used(Abilities::SHIELD)) {
         CApplication::get().getSoundManager().registerAndPlaySound(ITEM_PICKUP_SOUND);
         Level.getPlayer().incrementHealth();
         removeFromGame();
         Dead = true;
      }
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementItemHealth::updatePhysicsEngineObject(float time) {
   return; //It just falls if it's in the air, usually
}

//This is where the renderable would be updated for the more complex enemies
void CElementItemHealth::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter(), 0));
   SceneObject->setRotation(SVector3f(-90, 0, 90 + 140*ElapsedTime));
}

void CElementItemHealth::printInformation() {
   printf("CElementItemHealth; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
