#include "CElementItemSeed.h"
#include "CGameLevel.h"

CElementItemSeed::CElementItemSeed(SRect2 nArea) :
   CElementItem(nArea, Items::SEED) {
   ElapsedTime = rand() % 800;
}

void CElementItemSeed::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   //TODO: Use singleton class
   //PhysicsEngineObject = CCollisionEngine::get().addActor();
   PhysicsEngineObject = (new CCollisionEngine())->addActor(); //just to compile
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_ITEM;
   PhysicsEngineObject->CollideableLevel = INTERACTOR_ITEMS;
   PhysicsEngineObject->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
   PhysicsEngineObject->getAttributes().Bounce = 3;
}

void CElementItemSeed::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/seed.3ds");
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
   SceneObject->setScale(SVector3(.3f));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemSeed::OnCollision(CCollideable *Object, CCollideable *With) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementItemSeed::updatePhysicsEngineObject(float time) {
   return; //It just falls if it's in the air, usually
}

//This is where the renderable would be updated for the more complex enemies
void CElementItemSeed::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(PhysicsEngineObject->getArea().getCenter() + SVector2(0.0f, 0.1f), 0));
   SceneObject->setRotation(SVector3(-90, 0, 90 + 140*ElapsedTime));
}

void CElementItemSeed::printInformation() {
   printf("CElementItemSeed; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
