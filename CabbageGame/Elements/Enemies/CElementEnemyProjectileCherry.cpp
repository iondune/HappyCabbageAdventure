#include "CElementEnemyProjectileCherry.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileCherry::CElementEnemyProjectileCherry(SRect2f nArea, float xDirection)
: CElementEnemyProjectile(nArea, Enemies::CHERRY_PROJECTILE), XDir(xDirection), SetImpulse(true) {
   MaxHealth = 2;
   CurHealth = MaxHealth;
}


void CElementEnemyProjectileCherry::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getActorAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getActorAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getActorAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getActorAttributes().AirSpeedFactor = 1.0f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyProjectileCherry::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0)
	   mesh = CMeshLoader::load3dsMesh("Base/single_cherry.3ds");

   else if (Level.getEnvironment() == 1)
	   mesh = CMeshLoader::load3dsMesh("Base/desert_single_cherry.3ds");

   else if (Level.getEnvironment() == Env::WATER)
	   mesh = CMeshLoader::load3dsMesh("Base/water_single_cherry.3ds");

   else {
      mesh = CMeshLoader::load3dsMesh("Base/single_cherry.3ds");
   }


   if (mesh) {
      mesh->resizeMesh(SVector3f(.65f));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "CherryProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3f(1, 1, 1));

   SceneObject->setRotation(SVector3f(-90.0, 0.0, 0.0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileCherry::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));
}

void CElementEnemyProjectileCherry::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   if (SetImpulse) {
      float xRandom = 10.0f * frand();
      float yRandom = 10.0f * frand();

      if (XDir < 0)
         xRandom = -xRandom;

      PhysicsEngineObject->addImpulse(SVector2f(XDir + xRandom, 2.f + yRandom));
      SetImpulse = false;
   }
}

void CElementEnemyProjectileCherry::printInformation() {
   printf("CElementEnemyProjectileCherry; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
