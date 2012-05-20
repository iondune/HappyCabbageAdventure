#include "CElementEnemyProjectileBanana.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileBanana::CElementEnemyProjectileBanana(SRect2f nArea)
: CElementEnemyProjectile(nArea, Enemies::BANANA_PROJECTILE), SinValue(0.0f), CosValue(-1.0f) {
}


void CElementEnemyProjectileBanana::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->setControlFall(false);
   PhysicsEngineObject->setGravity(0.0f);

   PhysicsEngineObject->getAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyProjectileBanana::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
	   mesh = CMeshLoader::load3dsMesh("Base/banana_projectile.3ds");
   else if (Level.getEnvironment() == Env::DESERT)
	   mesh = CMeshLoader::load3dsMesh("Base/banana_projectile.3ds");
   else if (Level.getEnvironment() == Env::WATER)
	   mesh = CMeshLoader::load3dsMesh("Base/water_banana_projectile.3ds");
   else
	   mesh = CMeshLoader::load3dsMesh("Base/banana_projectile.3ds");

   if (mesh) {
      mesh->resizeMesh(SVector3f(1.0f));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "BananaProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3f(1, 1, 1));

   if(Direction == Projectile::LEFT)
      Rotation = SVector3f(-90, 0, -45);
   else if (Direction == Projectile::RIGHT)
      Rotation = SVector3f(-90, 0, 45);

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileBanana::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));

   if (Direction == Projectile::LEFT) {
      if (Level.getEnv() != Env::WATER)
         Rotation.Y+=2.4f;
      else
         Rotation.Y+=1.2f;
   }

   else {
      if (Level.getEnv() != Env::WATER)
         Rotation.Y-=2.4f;
      else
         Rotation.Y-=1.2f;
   }

   SceneObject->setRotation(Rotation);
}

void CElementEnemyProjectileBanana::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   printf("SinValue: %f\n", SinValue);

   if (Direction == Projectile::LEFT) { //go left
      if (Level.getEnv() != Env::WATER)
         PhysicsEngineObject->setVelocity(SVector2f(-5.0f*cos(CosValue), 2.5f*sin(SinValue)));
      else
         PhysicsEngineObject->setVelocity(SVector2f(-2.5f*cos(CosValue), 1.25f*sin(SinValue)));
   }
   
   else { //go right
      if (Level.getEnv() != Env::WATER)
         PhysicsEngineObject->setVelocity(SVector2f(5.0f*cos(CosValue), 2.5f*sin(SinValue)));
      else
         PhysicsEngineObject->setVelocity(SVector2f(2.5f*cos(CosValue), 1.25f*sin(SinValue)));
   }

   SinValue += time;
   CosValue += time;
}

void CElementEnemyProjectileBanana::printInformation() {
   printf("CElementEnemyProjectileBanana; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
