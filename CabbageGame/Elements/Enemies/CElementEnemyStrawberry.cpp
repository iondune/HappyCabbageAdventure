#include "CElementEnemyStrawberry.h"
#include "CGameLevel.h"

CElementEnemyStrawberry::CElementEnemyStrawberry(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::STRAWBERRY), ISquishable(nArea.Size.X, nArea.Size.Y), HitPlayer(false), OldPositionX(nArea.Position.X), JumpTimer(3.0f), JumpNum(Strawberry::NO_JUMP) {
}

void CElementEnemyStrawberry::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 1.0f;//2.8f;
   PhysicsEngineObject->getAttributes().JumpLength = .01f;
   PhysicsEngineObject->getAttributes().JumpAccel = 7.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;
   PhysicsEngineObject->getAttributes().AirControl = 0.75f;

   PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyStrawberry::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
      mesh = CMeshLoader::load3dsMesh("Base/strawberry.3ds");

   else if (Level.getEnvironment() == Env::DESERT)
      mesh = CMeshLoader::load3dsMesh("Base/strawberry.3ds");

   else if (Level.getEnvironment() == Env::WATER)
	   mesh = CMeshLoader::load3dsMesh("Base/water_strawberry.3ds");

   else
      mesh = CMeshLoader::load3dsMesh("Base/strawberry.3ds");

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
   SceneObject->setScale(SVector3f(Area.Size.X, Area.Size.X, Area.Size.Y));
   SceneObject->setRotation(SVector3f(-90, 0, -30));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

/*
void CElementEnemyStrawberry::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject())
      HitPlayer = true;
   CElementEnemy::OnCollision(Object);
}
*/

//This is where the AI would be updated for more complex enemies
void CElementEnemyStrawberry::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   float difference = Area.Position.X - OldPositionX;

   if (difference < .00001f && difference > -.00001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
      else if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }

   JumpTimer -= time;

   if (JumpTimer <= 0.0f) {
      if (JumpNum == Strawberry::NO_JUMP) {
         JumpNum = Strawberry::FIRST_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 3.5f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = .8f;
      }

      else if (JumpNum == Strawberry::FIRST_JUMP) {
         JumpNum = Strawberry::SECOND_JUMP;

         PhysicsEngineObject->setJumping(true);
         JumpTimer = .8f;
      }

      else if (JumpNum == Strawberry::SECOND_JUMP) {
         JumpNum = Strawberry::THIRD_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 8.5f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = 2.0f;
      }

      else if (JumpNum == Strawberry::THIRD_JUMP) {
         JumpNum = Strawberry::FOURTH_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 5.5f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = 1.2f;
      }
      else if (JumpNum == Strawberry::FOURTH_JUMP) {
         JumpNum = Strawberry::FIFTH_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 7.0f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = .8f;
      }
      else if (JumpNum == Strawberry::FIFTH_JUMP) {
         JumpNum = Strawberry::SIXTH_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 9.0f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = 2.0f;
      }
      else if (JumpNum == Strawberry::SIXTH_JUMP) {
         JumpNum = Strawberry::NO_JUMP;

         PhysicsEngineObject->getAttributes().JumpAccel = 12.0f;
         PhysicsEngineObject->setJumping(true);
         JumpTimer = 6.0f;
      }
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyStrawberry::updateSceneObject(float time) {
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

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
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

void CElementEnemyStrawberry::printInformation() {
   printf("CElementEnemyStrawberry; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
