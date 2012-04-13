#include "EApple.h"

EApple::EApple(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();

   PositiveScale = rollingLeft = rollingRight = false;
   rotate = 0.0f;

   ScaleMult = 1.0f;
   Time = 0.f;
}

//Loads and moves the mesh
void EApple::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   //CMesh *mesh = CMeshLoader::loadAsciiMesh("Base/Tyra.obj");
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
   Renderable->setTranslation(SVector3((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3(1, 1, 1));
   Renderable->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void EApple::loadActor() {
   Actor = Manager->getEngine()->addActor();
   Actor->setArea(SRect2(SVector2(x, y), SVector2(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 2.2f;
   Actor->CollideableType = COLLIDEABLE_TYPE_APPLE;
}

//Updates AI's decision per frame
void EApple::update(float const TickTime) {
   Time = TickTime;
    
   if (Manager->isPlayerAlive())
   {
       if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X && (!rollingLeft && !rollingRight))
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
       else if (Manager->getPlayerLocation().X > Actor->getArea().getCenter().X && (!rollingLeft && !rollingRight))
           Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);

       float curTime = TickTime;
       for(; curTime > 0; curTime -= 0.002f) { 
          if (Manager->getPlayerLocation().X - Actor->getArea().getCenter().X < 7 && Manager->getPlayerLocation().X - Actor->getArea().getCenter().X > -7 &&
                Manager->getPlayerLocation().Y - Actor->getArea().getCenter().Y < 1.0f && Manager->getPlayerLocation().Y - Actor->getArea().getCenter().Y > -1.0f && rand()%1000 <= 2) {
             if (Actor->getAction() == Cabbage::Collider::CActor::EActionType::MoveLeft)
                rollingLeft = true;
             else
                rollingRight = true;
             Actor->getAttributes().MaxWalk = 6.0f;
          }
       }

      if (rollingLeft) {
         rotate -= (.6f *  1000 * TickTime);
         Renderable->setRotation(SVector3(-90, rotate, 0));
      }

      if (rollingRight) {
         rotate += (.6f * 1000 * TickTime);
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

void EApple::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));

   if (ScaleMult > 1.1f)
      PositiveScale = false;
   else if (ScaleMult < .9f)
      PositiveScale = true;

   if (PositiveScale)
      ScaleMult += .35f * Time;
   else
      ScaleMult -= .35f * Time;

   ScaleMult = std::max(std::min(ScaleMult, 1.2f), 0.8f);

   if (!rollingLeft && !rollingRight) {
      if(Actor->getVelocity().X < -0.01f)
         Renderable->setScale(SVector3(-1,1,ScaleMult));
      else if(Actor->getVelocity().X > 0.01f)
         Renderable->setScale(SVector3(1,1,ScaleMult));
   }
   
   else {
      if(Actor->getVelocity().X < -0.01f)
         Renderable->setScale(SVector3(-1,1,1));
      else if(Actor->getVelocity().X > 0.01f)
         Renderable->setScale(SVector3(1,1,1));
   }
}
