#include "CElementItemHealth.h"
#include "CGameLevel.h"

CElementItemHealth::CElementItemHealth(SRect2 nArea) :
   CElementItem(nArea, Items::HEALTH) {

}

void CElementItemHealth::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_ITEM;
   PhysicsEngineObject->CollideableLevel = INTERACTOR_ITEMS;
   PhysicsEngineObject->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
}

void CElementItemHealth::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/healthboost.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   //SceneObject->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //SceneObject->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   SceneObject->setScale(SVector3(.8f));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemHealth::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      if(Level.getPlayer().getStats().Health < 5)
         Level.getPlayer().getStats().Health++;
      Level.removeItem(this);
      Area.Position.Y = 5.0f;
      Level.addItem(CItemLoader::LoadItem(Area, Items::HEALTH));
      removeFromEngines();
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
   SceneObject->setTranslation(SVector3(PhysicsEngineObject->getArea().getCenter(), 0));
   SceneObject->setRotation(SVector3(-90, 0, 90 + 140*ElapsedTime));
}

void CElementItemHealth::printInformation() {
   printf("CElementItemHealth; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
