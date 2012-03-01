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

   Time = 0.f;
}

//Loads and moves the mesh
void CIHealth::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/healthboost.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setShader("Toon");
   //Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(0.4f));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void CIHealth::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y) + h*0.4f, SVector2(w, h)*0.8f));

   //Set actor attributes
   Actor->CollideableType = COLLIDEABLE_TYPE_ITEM;
   Actor->CollideableLevel = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
}

//Updates AI's decision per frame
void CIHealth::update(float const TickTime) {
   Time += 140*TickTime;
   if(Time >= 180.0f)
      Time = 0;
}

void CIHealth::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y + 0.1f, 0));
   Renderable->setRotation(SVector3(-90, 0, 90 + Time));
}
