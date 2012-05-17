#include "EBlade.h"

EBlade::EBlade(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   BladeRotate= 0.0f;
   Time = 0.0f;
}

//Loads and moves the mesh
void EBlade::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/trap1.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   //Renderable->getMaterial().Texture = new CTexture(CImageLoader::loadImage("Textures/kiwi.bmp"));
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   //Renderable->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3f(2, 2, 2));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
   //Blades weren't turning because I had the hierarchy skip updating for immobile objects.
   //CApplication::get().getSceneManager().addImmobileSceneObject(Renderable, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
}

//Adds actor to engine and preps engine
void EBlade::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setFallAcceleration(0.0f);

   Actor->setArea(SRect2f(SVector2f(x, y-1.0f), SVector2f(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 3.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
}


//Updates AI's decision per frame
void EBlade::update(float const TickTime) {
   Time = TickTime;
}

void EBlade::doRenderable() {

   BladeRotate -= 300.0f * Time;

   Renderable->setRotation(SVector3f(-90, 0, BladeRotate));
   //Renderable->setScale(SVector3f(2.0f));
   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X, Actor->getArea().getCenter().Y, 0));
}
