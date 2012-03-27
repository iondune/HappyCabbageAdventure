#include "EGrape.h"

EGrape::EGrape(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   Time = 0.f;
}

//Loads and moves the mesh
void EGrape::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/alien.3ds");
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
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(0, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void EGrape::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 2.2f;
   Actor->CollideableType = COLLIDEABLE_TYPE_GRAPE;
}

//Updates AI's decision per frame
void EGrape::update(float const TickTime) {
   Time += TickTime;

   //printf("Time is: %f\n", Time);

   if (Time - 2.f > 0.f && Manager->isPlayerAlive()) {
      Time = 0.f;

      //fire projectile
      ThrowGrape();
   }
}

void EGrape::doRenderable() {
   //Face the proper direction
   if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
            Renderable->setRotation(SVector3(-90, 0, 90));
         else
            Renderable->setRotation(SVector3(-90, 0, -90));
}

void EGrape::ThrowGrape() {
   SVector2 pos = Actor->getArea().Position;

   printf("Firing a grape\n");

   if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X) //spawn to the left
      CBadGuy::makeBadGuy(pos.X - w/2.f - .05f, pos.Y, w, h, pGrape, Manager);
   else //spawn to the right
      CBadGuy::makeBadGuy(pos.X + w/2.f + .5f, pos.Y, w, h, pGrape, Manager);
}