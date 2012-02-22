#include "EOrange.h"

#include "EApple.h"

EOrange::EOrange(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   jump = true;
   oldX = x;

}

//Loads and moves the mesh
void EOrange::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setTexture("Textures/orange.bmp");

   Renderable->setShader("Toon");
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void EOrange::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 5.0f;
   Actor->getAttributes().WalkAccel = 4.0f;
   Actor->getAttributes().JumpAccel = 6.4f;
   Actor->getAttributes().JumpLength = .1f;
   Actor->getAttributes().AirControl = 0.75f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
}

//Updates AI's decision per frame
void EOrange::update(const float TickTime) {
   if (Manager->isPlayerAlive())
   {
       if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
       else
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);

       //if (Actor->getArea().getCenter().Y < Manager->getPlayerLocation().Y - 1.4 && !Manager->isJumping()) {
       //   Actor->setJumping(true);
       //}


       if (Manager->getPlayerLocation().X - Actor->getArea().getCenter().X < 2.2 &&
                Manager->getPlayerLocation().X - Actor->getArea().getCenter().X > -2.2) {
          if (jump) {
             Actor->setJumping(true);
             jump = false;
          }
       }
       else
          jump = true;
   }
   else
   {
       Actor->setAction(Cabbage::Collider::CActor::EActionType::None);
   }

   oldX = Actor->getArea().getCenter().X;
}

