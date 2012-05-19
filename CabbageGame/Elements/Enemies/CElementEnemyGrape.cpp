#include "CElementEnemyGrape.h"
#include "CGameLevel.h"

CElementEnemyGrape::CElementEnemyGrape(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::GRAPE), shootTime(0.f) {
}

void CElementEnemyGrape::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 0.0f;

   CElementEnemy::setupPhysicsEngineObject();
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

   else
      mesh = CMeshLoader::load3dsMesh("Base/grape_bunch.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(2));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y + .2f, 0));
   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyGrape::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   shootTime += time;

   //TODO: Check the player is alive
   if ((Level.getEnv() != Env:: WATER && shootTime >= 1.5f) || (Level.getEnv() == Env::WATER && shootTime >= 3.0f)) {
      shootTime = 0.f;

      //fire projectile
      ShootGrape();
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyGrape::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y + .2f, 0));

   SVector2f playerPosition = Level.getPlayer().getArea().Position;

   SceneObject->setRotation(SVector3f(-90, 0, 90));

   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 90));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
      return;
   }
   if(TimeToDeath > 0.0f) {
      CElementEnemy::updateSceneObject(time);
      return;
   }

}

void CElementEnemyGrape::printInformation() {
   printf("CElementEnemyGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyGrape::ShootGrape() {
   SVector2f playerPosition = Level.getPlayer().getArea().Position;
   float x, y;

   if (playerPosition.X < Area.Position.X) {//spawn to the left
      x = Area.Position.X - Area.Size.X/2.f - .5f;
      y = Area.Position.Y + .15f;

      Level.addEnemy(CEnemyLoader::LoadEnemy(SRect2f(x, y, Area.Size.X, Area.Size.Y), Enemies::GRAPE_PROJECTILE));
   }
   else {//spawn to the right
      x = Area.Position.X + Area.Size.X/2.f + .55f;
      y = Area.Position.Y + .15f;

      Level.addEnemy(CEnemyLoader::LoadEnemy(SRect2f(x, y, Area.Size.X, Area.Size.Y), Enemies::GRAPE_PROJECTILE));
   }
}
