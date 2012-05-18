#include "CElementEnemyPineapple.h"
#include "CGameLevel.h"

CElementEnemyPineapple::CElementEnemyPineapple(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::PINEAPPLE), ISquishable(4.0f, 4.0f), HitPlayer(false), OldPositionX(nArea.Position.X) {

   MaxHealth = 4;
   CurHealth = MaxHealth;
}

void CElementEnemyPineapple::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 1.2f;
   PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyPineapple::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/pineapple_stack.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/pineapple_stack.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/pineapple_stack.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3(-90, 0, 180));

   Scale = Area.Size;

   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

void CElementEnemyPineapple::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
      HitPlayer = true;

      //Check if jumped on top of enemy.
      if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f) {
         takeDamage(1);
      }

      //Did the player run into them?
      else {
         if(Level.getPlayer().decrementHealth()) {
            if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
               PlayerActor->setImpulse(SVector2(7.f, 2.8f), 0.1f);
            else
               PlayerActor->setImpulse(SVector2(-7.f, 2.8f), 0.1f);
            Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
   }
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyPineapple::updatePhysicsEngineObject(float time) {
   float difference = Area.Position.X - OldPositionX;

   if (difference < .00001f && difference > -.00001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
      else if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyPineapple::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3(-90, 0, 0));
      SceneObject->setScale(SVector3(1.0f, 1.0f, 0.3f));
      return;
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   //printf("Scale: %f, %f\n", Scale.X, Scale.Y);

   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(Scale.X,Scale.X,Scale.Y));
   }

   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(Scale.X,Scale.X,Scale.Y));
   }
}

void CElementEnemyPineapple::printInformation() {
   printf("CElementEnemyPineapple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
