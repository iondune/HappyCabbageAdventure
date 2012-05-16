#include "CElementEnemyPear.h"
#include "CGameLevel.h"

CElementEnemyPear::CElementEnemyPear(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::PEAR), ISquishable(nArea.Size.X, nArea.Size.Y), OldPositionX(nArea.Position.X), HitPlayer(false) {
   printf("Made a pear!\n");
}

void CElementEnemyPear::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;

   PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
}

void CElementEnemyPear::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");

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
   SceneObject->setRotation(SVector3f(-90, 0, 20));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyPear::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
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
               PlayerActor->addImpulse(SVector2f(7.f, 2.8f));
            else
               PlayerActor->addImpulse(SVector2f(-7.f, 2.8f));
            Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
   }
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyPear::updatePhysicsEngineObject(float time) {
   float difference = Area.Position.X - OldPositionX;

   if (difference < .001f && difference > -.001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
      else if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyPear::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
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

void CElementEnemyPear::printInformation() {
   printf("CElementEnemyPear; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
