#include "CElementItemLife.h"
#include "CGameLevel.h"

CElementItemLife::CElementItemLife(SRect2f nArea) :
   CElementItem(nArea, Items::LIFE) {

}

void CElementItemLife::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/crappycabbage2.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(0.4f));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter() + SVector2f(0.0f, 0.1f), 0));
   //SceneObject->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //SceneObject->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2, 0));
   SceneObject->setScale(SVector3f(.8f));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemLife::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject() && !Level.getPlayer().used(Abilities::SHIELD)) {
         CApplication::get().getSoundManager().registerAndPlaySound(ITEM_PICKUP_SOUND);
         Level.getPlayer().incrementLives();
         removeFromGame();
         Dead = true;
      }
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementItemLife::updatePhysicsEngineObject(float time) {
   return; //It just falls if it's in the air, usually
}

//This is where the renderable would be updated for the more complex enemies
void CElementItemLife::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter() + SVector2f(0.0f, 0.1f), 0));
   SceneObject->setRotation(SVector3f(-90, 0, 90 + 140*ElapsedTime));
}

void CElementItemLife::printInformation() {
   printf("CElementItemLife; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
