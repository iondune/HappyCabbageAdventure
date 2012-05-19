#include "CElementEnemyProjectileCherry.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileCherry::CElementEnemyProjectileCherry(SRect2 nArea, float xDirection)
: CElementEnemyProjectile(nArea, Enemies::CHERRY_PROJECTILE), XDir(xDirection), SetImpulse(true) {
   MaxHealth = 2;
   CurHealth = MaxHealth;
}


void CElementEnemyProjectileCherry::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyProjectileCherry::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/single_cherry.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/desert_single_cherry.3ds");
   }

   else {
      mesh = CMeshLoader::load3dsMesh("Base/single_cherry.3ds");
   }


   if (mesh) {
      mesh->resizeMesh(SVector3(.65f));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "CherryProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3(1, 1, 1));

   SceneObject->setRotation(SVector3(-90.0, 0.0, 0.0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileCherry::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y, 0));
}

void CElementEnemyProjectileCherry::updatePhysicsEngineObject(float time) {
   if (SetImpulse) {
      float xRandom = 10.0f * frand();
      float yRandom = 10.0f * frand();

      if (XDir < 0)
         xRandom = -xRandom;

      PhysicsEngineObject->setImpulse(SVector2(XDir + xRandom, 2.f + yRandom), 0.02f);
      SetImpulse = false;
   }
}

void CElementEnemyProjectileCherry::printInformation() {
   printf("CElementEnemyProjectileCherry; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
