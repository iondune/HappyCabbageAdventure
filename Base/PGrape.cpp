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
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(.5));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   Renderable->setScale(SVector3(1.0f, 1.0f, 1.0f));
   Renderable->setRotation(SVector3(-90, 0, 90));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void PGrape::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setFallAcceleration(0.0f);

   Actor->setArea(SRect2(SVector2(x, y), SVector2(w*.5f, h*.5f)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 4.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
   Actor->CollideableType = COLLIDEABLE_TYPE_PKIWI;

   Mix_PlayChannel(-1, dropKiwi, 0);
}

//Updates AI's decision per frame
void PGrape::update(float const TickTime) {

   Actor->setAction(Cabbage::Collider::CActor::EActionType::None);

   if (Manager->isPlayerAlive())
   {
      if (!Direction) //go left
         Actor->setVelocity(SVector2(-5.f, 0.f));
      else //go right
         Actor->setVelocity(SVector2(5.f, 0.f));

   }
}
