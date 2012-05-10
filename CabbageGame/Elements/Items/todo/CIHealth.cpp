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
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/healthboost.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   //Renderable->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2 + 10.6f, 0));
   //Renderable->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3f(.8f));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void CIHealth::loadActor() {
   Actor = Manager->getEngine()->addActor();
   //Actor->setArea(SRect2f(SVector2f(x, y) , SVector2f(w, h)*0.5f));
   Actor->setArea(SRect2f((float)x, (float)y, 1, 1));

   //Set actor attributes
   Actor->CollideableLevel = INTERACTOR_ITEMS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
}

//Updates AI's decision per frame
void CIHealth::update(float const TickTime) {
   Time += 140*TickTime;
   if(Time >= 360.0f)
      Time = 0;
}

void CIHealth::doRenderable() {
   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y , 0));
   Renderable->setRotation(SVector3f(-90, 0, 90 + Time));
}
