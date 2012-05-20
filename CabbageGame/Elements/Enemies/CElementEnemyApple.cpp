#include "CElementEnemyApple.h"
#include "CGameLevel.h"

CElementEnemyApple::CElementEnemyApple(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::APPLE), PositiveScale(false), ScaleMult(0.0f), Roll(None), Rotate(0.0f), ISquishable(1.0f, 1.0f) {

}

void CElementEnemyApple::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyApple::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST) {
      mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   }

   else if (Level.getEnvironment() == Env::DESERT) {
      mesh = CMeshLoader::load3dsMesh("Base/desertapple.3ds");
   }
   else if (Level.getEnvironment() == Env::WATER)
      mesh = CMeshLoader::load3dsMesh("Base/water_apple.3ds");
   else
      mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y-1))/2, 0));
   SceneObject->setScale(SVector3f(1, 1, 1));
   SceneObject->setRotation(SVector3f(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyApple::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   SVector2f PlayerPosition = Level.getPlayer().getArea().Position;
   //TODO: Make some class singleton so we can get the player's location
   if (PlayerPosition.X < Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (PlayerPosition.X > Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

   float curTime = time;
   for(; curTime > 0; curTime -= 0.002f) { 
      if (PlayerPosition.X - Area.getCenter().X < 7 && PlayerPosition.X - Area.getCenter().X > -7 &&
            PlayerPosition.Y - Area.getCenter().Y < 1.0f && PlayerPosition.Y - Area.getCenter().Y > -1.0f && rand()%1000 <= 2) {
         if(PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
            Roll = Left;
         else
            Roll = Right;
         if (Level.getEnv() != Env::WATER)
                  PhysicsEngineObject->getAttributes().MaxWalk = 6.0f;
               else
                  PhysicsEngineObject->getAttributes().MaxWalk = 3.0f;
      }
   }

   if (Rotate >= 360 || Rotate <= -360) {
      Rotate = 0;
      Roll = None;
      if (Level.getEnv() != Env::WATER)
         PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
      else
         PhysicsEngineObject->getAttributes().MaxWalk = 1.1f;
      PhysicsEngineObject->setVelocity(SVector2f(2.2f, 0.0f));
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyApple::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
      return;
   }
   if(TimeToDeath > 0.0f) {
      CElementEnemy::updateSceneObject(time);
      return;
   }

   switch(Roll) {
   case Left:
      Rotate -= (.6f *  1000 * time);
      SceneObject->setRotation(SVector3f(-90, Rotate, 0));
      break;
   case Right:
      Rotate += (.6f * 1000 * time);
      SceneObject->setRotation(SVector3f(-90, Rotate, 0));
      break;
   default:
      break;
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   if (ScaleMult > 1.1f)
      PositiveScale = false;
   else if (ScaleMult < .9f)
      PositiveScale = true;

   if (PositiveScale)
      ScaleMult += .35f * time;
   else
      ScaleMult -= .35f * time;

   ScaleMult = std::max(std::min(ScaleMult, 1.2f), 0.8f);

   Scale.Y += ScaleMult - 1.0f;

   if (Roll == None && PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3f(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3f(-1,1,1));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(1,1,1));
   }
}

void CElementEnemyApple::printInformation() {
   printf("CElementEnemyApple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
