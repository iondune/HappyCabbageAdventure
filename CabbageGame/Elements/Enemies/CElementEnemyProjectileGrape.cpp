#include "CElementEnemyProjectileGrape.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileGrape::CElementEnemyProjectileGrape(SRect2f nArea)
: CElementEnemyProjectile(nArea, Enemies::GRAPE_PROJECTILE) {
}


void CElementEnemyProjectileGrape::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->setGravityEnabled(false);

   PhysicsEngineObject->getActorAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getActorAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getActorAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getActorAttributes().AirSpeedFactor = 1.0f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyProjectileGrape::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   int random = rand() % 3;

   if (Level.getEnvironment() == Env::FOREST) {
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/grape3.3ds");
   }
   else if (Level.getEnvironment() == Env::DESERT) {
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/desert_grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/desert_grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/desert_grape3.3ds");
   }
   else if (Level.getEnvironment() == Env::WATER) {
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/water_grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/water_grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/water_grape3.3ds");
   }
   else { //Default to forest
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/grape3.3ds");
   }


   if (mesh) {
      mesh->resizeMesh(SVector3f(.4f));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "GrapeProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3f(1, 1, 1));

   if(Level.getPlayer().getArea().Position.X < Area.Position.X) {
      SceneObject->setRotation(SVector3f(-90, 0, -45));
      PlayerLeft = true;
   }
   else {
      SceneObject->setRotation(SVector3f(-90, 0, 45));
      PlayerLeft = false;
   }

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileGrape::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));
}

void CElementEnemyProjectileGrape::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if (PlayerLeft) { //go left
      if (Level.getEnv() != Env::WATER)
         PhysicsEngineObject->setVelocity(SVector2f(-5.f, 0.f));
      else
         PhysicsEngineObject->setVelocity(SVector2f(-2.5f, 0.f));
   }
   
   else { //go right
      if (Level.getEnv() != Env::WATER)
         PhysicsEngineObject->setVelocity(SVector2f(5.f, 0.f));
      else
         PhysicsEngineObject->setVelocity(SVector2f(2.5f, 0.f));
   }
}

void CElementEnemyProjectileGrape::printInformation() {
   printf("CElementEnemyProjectileGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
