#include "EKiwi.h"

EKiwi::EKiwi(float x, float y, float w, float h, CGameplayManager* manager, int direction) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   Direction = direction;

   OrigX = x;

   rotateBird = 0.0f;
}

//Loads and moves the mesh
void EKiwi::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/killerkiwi.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   //Renderable->getMaterial().Texture = new CTexture(CImageLoader::loadImage("Textures/kiwi.bmp"));
   Renderable->setShader("Toon");
   //Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, -90));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void EKiwi::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setFallAcceleration(0.0f);

   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 3.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
}

//Updates AI's decision per frame
void EKiwi::update(float const TickTime) {
   if (Manager->isPlayerAlive())
   {
      float curX = Actor->getArea().Position.X;
      SineValue = sin(curX - OrigX);

      float TempY = y + SineValue;

      Actor->setArea(SRect2(curX, TempY, w, h));

      Actor->setAction(Cabbage::Collider::CActor::EActionType::None);

      if(Direction == 0)
         Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
      else
         Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);
   }
   else
   {
       Actor->setAction(Cabbage::Collider::CActor::EActionType::None);
   }
}

void EKiwi::doRenderable() {

      rotateBird = -100.0f * SineValue;

   printf("rotateBird: %f\n", rotateBird);

   Renderable->setRotation(SVector3(-90 + rotateBird, 0, -90));

   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));

   if(Actor->getVelocity().X < -0.01f)
      Renderable->setScale(SVector3(-1,1,1));
   else if(Actor->getVelocity().X > 0.01f)
      Renderable->setScale(SVector3(1,1,1));

}

