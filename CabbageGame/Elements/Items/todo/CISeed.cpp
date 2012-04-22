#include "CIHealth.h"

CIHealth::CIHealth(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   rotate = 0.0f;

   Time = (float) (rand() % 800);
}

//Loads and moves the mesh
void CIHealth::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/seed.3ds");
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
   //Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(0.3f));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void CIHealth::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->CollideableType = COLLIDEABLE_TYPE_ITEM;
   Actor->CollideableLevel = INTERACTOR_ITEMS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
   Actor->getAttributes().Bounce = 3;

   float rand1 = (float)rand()/(float)RAND_MAX;
   float rand2 = (float)rand()/(float)RAND_MAX;

   Actor->setImpulse(SVector2(rand1*8.f - 4.f, rand2*4.5f + 1.0f), 0.01f);
}

//Updates AI's decision per frame
void CIHealth::update(float const TickTime) {
   Time += 140*TickTime;
   if(Time >= 360.0f)
      Time = 0;
}

void CIHealth::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y + 0.1f, 0));
   Renderable->setRotation(SVector3(-90 + 2.f*Time, 0 + 1.f*Time, 90 + 4.f*Time));
}
