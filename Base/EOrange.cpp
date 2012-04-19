#include "EOrange.h"

EOrange::EOrange(float x, float y, float w, float h, CGameplayManager* manager, int env) {

   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   this->env = env;

   loadMesh();
   loadActor();

   jump = true;
   oldX = x;
}

//Loads and moves the mesh
void EOrange::loadMesh() {
   Renderable = new CMeshSceneObject();
   CMesh *mesh;

   if (env == 0)
      mesh = CMeshLoader::load3dsMesh("Base/orange.3ds");
   else if (env == 1)
      mesh = CMeshLoader::load3dsMesh("Base/desertorange.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Renderable->setMesh(mesh);
   Renderable->setTexture("Base/orange.bmp");
   
   Renderable->setShader(ERP_DEFAULT, "Toon");
   Renderable->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
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
   Actor->CollideableType = COLLIDEABLE_TYPE_ORANGE;
}

//Updates AI's decision per frame
void EOrange::update(const float TickTime) {
   if (Manager->isPlayerAlive())
   {
       if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
           Actor->setAction(CCollisionActor::EActionType::MoveLeft);
       else
           Actor->setAction(CCollisionActor::EActionType::MoveRight);

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
       Actor->setAction(CCollisionActor::EActionType::None);
   }

   oldX = Actor->getArea().getCenter().X;
}

void EOrange::doRenderable() {

   if (Actor->getVelocity().X < 0.0f)
      Renderable->setRotation(SVector3(-90, Actor->getVelocity().X*10.0f, -90));
   else if (Actor->getVelocity().X >= 0.0f)
      Renderable->setRotation(SVector3(-90, Actor->getVelocity().X*10.0f, 90));
   else {
      if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
         Renderable->setRotation(SVector3(-90, 0, -90));
      else
         Renderable->setRotation(SVector3(-90, 0, 90));
   
   }

   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));

   if(Actor->getVelocity().X < -0.01f)
      Renderable->setScale(SVector3(-1,1,1));
   else if(Actor->getVelocity().X > 0.01f)
      Renderable->setScale(SVector3(1,1,1));
}
