#include "PGrape.h"

PGrape::PGrape(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   if (Manager->getPlayerLocation().X <= x)
      Direction = false;
   else
      Direction = true;
}

//Loads and moves the mesh
void PGrape::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh;

   r = rand()%3;
   if (r == 0)
      mesh = CMeshLoader::load3dsMesh("Base/grape1.3ds");
   else if (r == 1)
      mesh = CMeshLoader::load3dsMesh("Base/grape2.3ds");
   else
      mesh = CMeshLoader::load3dsMesh("Base/grape3.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(.5));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   Renderable->setScale(SVector3f(1.0f, 1.0f, 1.0f));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void PGrape::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setFallAcceleration(0.0f);

   Actor->setArea(SRect2f(SVector2f(x, y), SVector2f(w*.5f, h*.5f)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 4.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;

   Mix_PlayChannel(-1, dropKiwi, 0);
}

//Updates AI's decision per frame
void PGrape::update(float const TickTime) {

   Actor->setAction(CCollisionActor::EActionType::None);

   if (Manager->isPlayerAlive())
   {
      if (!Direction) { //go left
         Actor->setVelocity(SVector2f(-5.f, 0.f));
         Renderable->setRotation(SVector3f(-90, 0, -45));
      }
      else { //go right
         Actor->setVelocity(SVector2f(5.f, 0.f));
         Renderable->setRotation(SVector3f(-90, 0, 45));
      }

   }
}
