#include "CElementEnemyPomegranate.h"
#include "CGameLevel.h"

CElementEnemyPomegranate::CElementEnemyPomegranate(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::POMEGRANATE), ISquishable(nArea.Size.X, nArea.Size.Y), OldPositionX(nArea.Position.X), HitPlayer(false), FlameTimer(0.0f) {

   particleEngine = new CParticleEngine(SVector3f(Area.getCenter().X - nArea.Size.X, Area.getCenter().Y + nArea.Size.Y/2.f, 0), 100, -1, FLAME_PARTICLE, Level.isNight());
   particleEngine->setVisible(false);
}

void CElementEnemyPomegranate::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getActorAttributes().MaxWalk = 2.2f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyPomegranate::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
	   mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");
   else if (Level.getEnvironment() == Env::DESERT)
	   mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");
   else if (Level.getEnvironment() == Env::WATER)
	   mesh = CMeshLoader::load3dsMesh("Base/water_pomegranate.3ds");
   else
      mesh = CMeshLoader::load3dsMesh("Base/pomegranate.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3f(-90, 0, 0));

   Scale = Area.Size;
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   SceneObject->setScale(SVector3f(Scale.X, Scale.X, Scale.Y));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

void CElementEnemyPomegranate::OnCollision(const SCollisionEvent& Event) {
   if(TimeToDeath > 0.0f)
      return;
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
      HitPlayer = true;

      //Check if jumped on top of enemy.
      if(!Dead && Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y - 0.1f && particleEngine && particleEngine->isVisible() == false) {
         if (CurHealth == 1) {
            particleEngine->deconstruct();
            delete particleEngine;
            particleEngine = NULL;
         }
         if (InvincibilityTime <= 0.0f) {
            takeDamage(1);
            printf("Took damage.\n");
            InvincibilityTime = .2f;
         }
      }

      //Did the player run into them?
      else if (CurHealth > 0){
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
void CElementEnemyPomegranate::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   float difference = Area.Position.X - OldPositionX;

   FlameTimer += time;

   if (fabs(difference) < 0.0001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
      else
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }

   if (FlameTimer >= 4.0f) {
      if (particleEngine->isVisible()) {
         HideFlame();
      }

      else {
         particleEngine->setVisible(true);
         PhysicsEngineObject->setArea(SRect2f(Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y + 1.000f));
      }

      FlameTimer = 0.0f;
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyPomegranate::updateSceneObject(float time) {
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(Scale.X, Scale.X, 0.3f));
      return;
   }
   if(TimeToDeath > 0.0f) {
      CElementEnemy::updateSceneObject(time);
      return;
   }

   if (particleEngine->isVisible()) {
      float xPos = Area.getCenter().X - Area.Size.X/2.0f;
      float yPos = Area.getCenter().Y;

      particleEngine->step(time);
      particleEngine->setCenterPos(SVector3f(xPos, yPos, 0));

      SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y - .5f, 0));
   }

   else {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
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
         SceneObject->setScale(SVector3f(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3f(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   }
}

void CElementEnemyPomegranate::HideFlame() {
   particleEngine->setVisible(false);

   PhysicsEngineObject->setArea(SRect2f(Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y - 1.000f));
}

void CElementEnemyPomegranate::printInformation() {
   printf("CElementEnemyApple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
