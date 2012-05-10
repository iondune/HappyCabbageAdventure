#include "CElementItemSeed.h"
#include "CGameLevel.h"

CElementItemSeed::CElementItemSeed(SRect2f nArea) :
   CElementItem(nArea, Items::SEED) {
   ElapsedTime = (float) (rand() % 800);
}

void CElementItemSeed::setupPhysicsEngineObject() {
   Area.Size = SVector2f(0.1f, 0.1f);
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->setTypeId(INTERACTOR_ITEMS);
   PhysicsEngineObject->setCollisionMask(INTERACTOR_BLOCKS);
   PhysicsEngineObject->getAttributes().Bounce = 3;
}

void CElementItemSeed::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/seed.3ds");
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
   //SceneObject->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //SceneObject->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2, 0));
   SceneObject->setScale(SVector3f(.3f));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemSeed::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject() && !Level.getPlayer().used(Abilities::SHIELD)) {
      Level.getPlayer().incrementSeeds();
      removeFromGame();
      Dead = true;
   }
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementItemSeed::updatePhysicsEngineObject(float time) {
   return; //It just falls if it's in the air, usually
}

//This is where the renderable would be updated for the more complex enemies
void CElementItemSeed::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter() + SVector2f(0.0f, 0.1f), 0));
   float Time = 140*ElapsedTime;
   SceneObject->setRotation(SVector3f(-90 + 2.f*Time, 0 + 1.f*Time, 90 + 4.f*Time));
}

void CElementItemSeed::printInformation() {
   printf("CElementItemSeed; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
