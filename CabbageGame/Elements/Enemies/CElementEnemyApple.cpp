#include "CElementEnemyApple.h"
#include "CGameLevel.h"

CElementEnemyApple::CElementEnemyApple(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::APPLE), PositiveScale(false), ScaleMult(0.0f), Roll(None), Rotate(0.0f) {

}

void CElementEnemyApple::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   //TODO: Use singleton class
   //PhysicsEngineObject = CCollisionEngine::get().addActor();
   PhysicsEngineObject = (new CCollisionEngine())->addActor(); //just to compile
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_APPLE;
}

void CElementEnemyApple::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/desertapple.3ds");
   }

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y-1))/2, 0));
   SceneObject->setScale(SVector3(1, 1, 1));
   SceneObject->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyApple::OnCollision(CCollideable *Object, CCollideable *With) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyApple::updatePhysicsEngineObject(float time) {
   //TODO: Make some class singleton so we can get the player's location
   /*
   if (Manager->getPlayerLocation().X < Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (Manager->getPlayerLocation().X > Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

   float curTime = time;
   for(; curTime > 0; curTime -= 0.002f) { 
      if (Manager->getPlayerLocation().X - Area.getCenter().X < 7 && Manager->getPlayerLocation().X - Area.getCenter().X > -7 &&
            Manager->getPlayerLocation().Y - Area.getCenter().Y < 1.0f && Manager->getPlayerLocation().Y - Area.getCenter().Y > -1.0f && rand()%1000 <= 2) {
         if(PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
            Roll = Left;
         else
            Roll = Right;
         PhysicsEngineObject->getAttributes().MaxWalk = 6.0f;
      }
   }
   */

   if (Rotate >= 360 || Rotate <= -360) {
      Rotate = 0;
      Roll = None;
      PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
      PhysicsEngineObject->setVelocity(SVector2(2.2f, 0.0f));
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyApple::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));

   switch(Roll) {
   case Left:
      Rotate -= (.6f *  1000 * time);
      SceneObject->setRotation(SVector3(-90, Rotate, 0));
      break;
   case Right:
      Rotate += (.6f * 1000 * time);
      SceneObject->setRotation(SVector3(-90, Rotate, 0));
      break;
   default:
      break;
   }

   if (ScaleMult > 1.1f)
      PositiveScale = false;
   else if (ScaleMult < .9f)
      PositiveScale = true;

   if (PositiveScale)
      ScaleMult += .35f * time;
   else
      ScaleMult -= .35f * time;

   ScaleMult = std::max(std::min(ScaleMult, 1.2f), 0.8f);

   if (Roll == None) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-1,1,ScaleMult));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(1,1,ScaleMult));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-1,1,1));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(1,1,1));
   }
}
