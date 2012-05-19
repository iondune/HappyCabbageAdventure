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
   bombDropped = false;

   Depth = 1.f;
   inZ = 0;
   zTimer = 0.0f;
   oldSineValue = 0.0f;
}

//Loads and moves the mesh
void EKiwi::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  KIWI MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   //Renderable->getMaterial().Texture = new CTexture(CImageLoader::loadImage("Textures/kiwi.bmp"));
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   //Renderable->setTranslation(SVector3f((x+(x+1))/2, (y+(y-1))/2, 0));
   Renderable->setScale(SVector3f(1, 1, 1));
   Renderable->setRotation(SVector3f(-90, 0, -90));

   CApplication::get().getSceneManager().addSceneObject(Renderable);
}

//Adds actor to engine and preps engine
void EKiwi::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setGravity(0.0f);

   Actor->setArea(SRect2f(SVector2f(x, y), SVector2f(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 3.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
   Actor->getAttributes().Reacts = 0;
   Actor->CollideableLevel = INTERACTOR_SUPERNONCOLLIDERS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;


}

#define Z_SPEED 0.2f
//Updates AI's decision per frame
void EKiwi::update(float const TickTime) {
   if (Manager->isPlayerAlive())
   {
      if(zTimer < 0.0f)
         zTimer = 0.0f;
      if(inZ) {
         if(zTimer >= Z_SPEED)
            zTimer = Z_SPEED; 
         else {
            zTimer += TickTime;
         }
      }
      else {
         if(zTimer > 0.0f)
            zTimer -= TickTime;
         else {
         }
      }
      float curX = Actor->getArea().Position.X;
      SineValue = 0.6f*sin(curX - OrigX);

      y = Actor->getArea().Position.Y;
      y -= oldSineValue;
      y += SineValue; 

      Actor->setArea(SRect2f(curX, y, w, h));


      Actor->setAction(CCollisionActor::EActionType::None);

      SVector2f vel = Actor->getVelocity();
      Actor->setVelocity(SVector2f(vel.X, vel.Y > 0 ? vel.Y - 1.0f*TickTime : 0));

      if(Direction == 0)
         Actor->setAction(CCollisionActor::EActionType::MoveLeft);
      else
         Actor->setAction(CCollisionActor::EActionType::MoveRight);
      oldSineValue = SineValue;

      if(!inZ) {
         float xDist = curX - Manager->getPlayerLocation().X;

         //Drop bomb projectile
         if (xDist < .2f && !bombDropped && Direction == 0) {
            printf("curX: %f, playerX: %f\n", curX, Manager->getPlayerLocation().X);
            printf("Dropping bomb\n");
            DropBomb();
            bombDropped = true;
            printf("Bomb dropped.\n");
         }
         else if (xDist > -.2f && !bombDropped && Direction == 1) {
            DropBomb();
            bombDropped = true;
         }
      }
   }
   else
   {
       Actor->setAction(CCollisionActor::EActionType::None);
   }
}

void EKiwi::doRenderable() {

   rotateBird = -100.0f * SineValue;

   Renderable->setRotation(SVector3f(-90 + rotateBird, 0, -90));

   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, zTimer*0.9f*Depth*(1.0f/Z_SPEED)));

   if(Actor->getVelocity().X < -0.01f)
      Renderable->setScale(SVector3f(-1,1,1)*(zTimer + 1.0f));
   else if(Actor->getVelocity().X > 0.01f)
      Renderable->setScale(SVector3f(1,1,1)*(zTimer + 1.0f));

}

void EKiwi::DropBomb() {
   SVector2f pos = Actor->getArea().Position;

   if (Direction == 0)
      CBadGuy::makeBadGuy(pos.X + w/2.f - .05f, pos.Y - .5f, w, h, pKiwi, Manager, 0);
   else
      CBadGuy::makeBadGuy(pos.X - w/2.f - .05f, pos.Y - .5f, w, h, pKiwi, Manager, 0);
}

