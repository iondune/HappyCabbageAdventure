#include "CElementEnemyPomegranite.h"
#include "CGameLevel.h"

CElementEnemyPomegranite::CElementEnemyPomegranite(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::POMEGRANITE), ISquishable(nArea.Size.X, nArea.Size.Y), OldPositionX(nArea.Position.X), HitPlayer(false), FlameTimer(0.0f) {

   MaxHealth = 1;
   CurHealth = MaxHealth;
}

void CElementEnemyPomegranite::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;

   PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
}

void CElementEnemyPomegranite::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3(-90, 0, 0));

   Scale = Area.Size;
   SceneObject->setScale(SVector3(Scale.X, Scale.X, Scale.Y));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

void CElementEnemyPomegranite::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
      HitPlayer = true;

      //Check if jumped on top of enemy.
      if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.05f && !particleEngine) {
         if (particleEngine && CurHealth == 1)
            HideFlame();
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
void CElementEnemyPomegranite::updatePhysicsEngineObject(float time) {
   float difference = Area.Position.X - OldPositionX;

   FlameTimer += time;

   if (difference < .001f && difference > -.001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveLeft)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
      else if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }

   if (FlameTimer >= 4.0f) {
      if (particleEngine) {
         HideFlame();
      }

      else {
         float xPos = Area.getCenter().X - Area.Size.X/2.0f;
         float yPos = Area.getCenter().Y + .5f;
         particleEngine = new CParticleEngine(SVector3(xPos, yPos, 0), 100, -1, FLAME_PARTICLE);
         PhysicsEngineObject->setArea(SRect2(Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y + 1.0f));
      }

      FlameTimer = 0.0f;
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyPomegranite::updateSceneObject(float time) {
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3(-90, 0, 0));
      SceneObject->setScale(SVector3(Scale.X, Scale.X, 0.3f));
      return;
   }

   if (particleEngine) {
      float xPos = Area.getCenter().X - Area.Size.X/2.0f;
      float yPos = Area.getCenter().Y;

      particleEngine->step(time);
      particleEngine->setCenterPos(SVector3(xPos, yPos, 0));

      SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y - .5f, 0));
   }

   else {
      SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));
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

void CElementEnemyPomegranite::HideFlame() {
   particleEngine->deconstruct();
   delete particleEngine;
   particleEngine = NULL;

   PhysicsEngineObject->setArea(SRect2(Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y - 1.0f));
}

void CElementEnemyPomegranite::printInformation() {
   printf("CElementEnemyApple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
