#include "PKiwi.h"

PKiwi::PKiwi(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();
}

//Loads and moves the mesh
void PKiwi::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   Renderable->setScale(SVector3(.5f, .5f, .5f));
   Renderable->setRotation(SVector3(-90, 90, 90));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void PKiwi::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setGravity(0.0f);

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
void PKiwi::update(float const TickTime) {

   Actor->setAction(CCollisionActor::EActionType::None);

   if (Manager->isPlayerAlive())
   {
      Actor->setVelocity(SVector2(0.f, -6.f));
   }
}
