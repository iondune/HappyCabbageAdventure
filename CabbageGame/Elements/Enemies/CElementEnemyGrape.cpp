#include "CElementEnemyGrape.h"
#include "CGameLevel.h"

CElementEnemyGrape::CElementEnemyGrape(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::GRAPE), shootTime(0.f) {

}

void CElementEnemyGrape::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 0.0f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_GRAPE;
}

void CElementEnemyGrape::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/grape_bunch.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/grape_bunch.3ds");
   }

   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/grape_bunch.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(2));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3(1, 1, 1));
   SceneObject->setRotation(SVector3(-90, 0, 90));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyGrape::updatePhysicsEngineObject(float time) {
   shootTime += time;

   //TODO: Check the player is alive
   if (shootTime - 2.f > 0.f) {
      shootTime = 0.f;

      //fire projectile
      ShootGrape();
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyGrape::updateSceneObject(float time) {
   SVector2 playerPosition = Level.getPlayer().getArea().Position;

   shootTime += time;

   SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y + .2f, 0));

   SceneObject->setRotation(SVector3(-90, 0, 90));

   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3(-90, 0, 90));
      SceneObject->setScale(SVector3(1.0f, 1.0f, 0.3f));
      return;
   }

}

void CElementEnemyGrape::printInformation() {
   printf("CElementEnemyGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyGrape::ShootGrape() {
   SVector2 playerPosition = Level.getPlayer().getArea().Position;
   float x, y;

   if (playerPosition.X < Area.Position.X) {//spawn to the left
      x = Area.Position.X - Area.Size.X/2.f - .5f;
      y = Area.Position.Y + .15f;

      Level.addEnemy(CEnemyLoader::LoadEnemy(SRect2(x, y, Area.Size.X, Area.Size.Y), Enemies::GRAPE_PROJECTILE));
   }
   else {//spawn to the right
      x = Area.Position.X + Area.Size.X/2.f + .55f;
      y = Area.Position.Y + .15f;

      Level.addEnemy(CEnemyLoader::LoadEnemy(SRect2(x, y, Area.Size.X, Area.Size.Y), Enemies::GRAPE_PROJECTILE));
   }
}
