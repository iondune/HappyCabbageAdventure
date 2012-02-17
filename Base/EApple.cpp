#include "EApple.h"

EApple::EApple(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   rollingLeft = rollingRight = false;
   rotate = 0.0f;
}

//Loads and moves the mesh
void EApple::loadMesh() {
   Renderable = new CMeshRenderable();
   CMesh *mesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->getMaterial().Shader = CShaderLoader::loadShader("Diffuse");
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addRenderable(Renderable);
}

//Adds actor to engine and preps engine
void EApple::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 2.2f;
}

//Updates AI's decision per frame
void EApple::update() {
   if (Manager->isPlayerAlive())
   {
       if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X && (!rollingLeft && !rollingRight))
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
       else if (Manager->getPlayerLocation().X > Actor->getArea().getCenter().X && (!rollingLeft && !rollingRight))
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);

       if (Manager->getPlayerLocation().X - Actor->getArea().getCenter().X < 7 && Manager->getPlayerLocation().X - Actor->getArea().getCenter().X > -7 && rand()%700 == 5) {
             if (Actor->getAction() == Cabbage::Collider::CActor::EActionType::MoveLeft)
                rollingLeft = true;
             else
                rollingRight = true;
             Actor->getAttributes().MaxWalk = 6.0f;
       }

      if (rollingLeft) {
         rotate -= 3;
         Renderable->setRotation(SVector3(-90, rotate, 0));
      }

      if (rollingRight) {
         rotate += 3;
         Renderable->setRotation(SVector3(-90, rotate, 0));
      }

      if (rotate >= 360 || rotate <= -360) {
         rotate = 0;
         rollingLeft = rollingRight = false;
         Actor->getAttributes().MaxWalk = 2.2f;
         Actor->setVelocity(SVector2(2.2f, 0.0f));
      }
   }
   else
   {
       Actor->setAction(Cabbage::Collider::CActor::EActionType::None);
   }
   //if (rand()%1000 == 1)
   //it->Actor->setJumping(true);
}
