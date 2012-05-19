#include "CElementEnemyBanana.h"
#include "CElementEnemyProjectile.h"
#include "CGameLevel.h"

CElementEnemyBanana::CElementEnemyBanana(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::BANANA), ISquishable(nArea.Size.X, nArea.Size.Y), ShootTime(0.0f) {
}

void CElementEnemyBanana::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 0.8f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyBanana::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/banana.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/banana.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/banana.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);

   Scale = Area.Size;
   SceneObject->setScale(SVector3f(Scale.X, Scale.X, Scale.Y));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyBanana::updatePhysicsEngineObject(float time) {
   SVector2f PlayerPosition = Level.getPlayer().getArea().Position;
   //TODO: Make some class singleton so we can get the player's location
   if (PlayerPosition.X < Area.getCenter().X)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (PlayerPosition.X > Area.getCenter().X)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

   ShootTime += time;

   if ((ShootTime >= 2.0f && Level.getEnv() != Env::WATER) || ShootTime >= 4.0f) {
      ShootTime = 0.0f;

      float x = Area.Position.X;
      float y = Area.Position.Y;
      CElementEnemy *projectile;

      if (PlayerPosition.X < Area.Position.X) {
         x -= 1.2f;
         projectile = CEnemyLoader::LoadEnemy(SRect2f(x, y, Area.Size.X, Area.Size.Y), Enemies::BANANA_PROJECTILE);
                  ((CElementEnemyProjectile *)projectile)->setDirection(Projectile::LEFT);
                  Level.addEnemy(projectile);
      }

      else {
         x += 1.2f;
         projectile = CEnemyLoader::LoadEnemy(SRect2f(x, y, Area.Size.X, Area.Size.Y), Enemies::BANANA_PROJECTILE);
         ((CElementEnemyProjectile *)projectile)->setDirection(Projectile::RIGHT);
         Level.addEnemy(projectile);
      }
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyBanana::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(Scale.X, Scale.X, 0.3f));
      return;
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

   SceneObject->setScale(SVector3f(Scale.X, Scale.X, Scale.Y));

   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setRotation(SVector3f(-90,0,-45));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setRotation(SVector3f(-90,0,45));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setRotation(SVector3f(-90,0,-45));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setRotation(SVector3f(-90,0,45));
   }
}

void CElementEnemyBanana::printInformation() {
   printf("CElementEnemyBanana; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
