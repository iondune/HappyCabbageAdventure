#include "CElementEnemyProjectileBanana.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileBanana::CElementEnemyProjectileBanana(SRect2 nArea)
: CElementEnemyProjectile(nArea, Enemies::BANANA_PROJECTILE), SinValue(0.0f), CosValue(-1.0f) {
}


void CElementEnemyProjectileBanana::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->setControlFall(false);
   PhysicsEngineObject->setFallAcceleration(0.0f);

   PhysicsEngineObject->getAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_PKIWI;
}

void CElementEnemyProjectileBanana::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
     mesh = CMeshLoader::load3dsMesh("Base/banana_projectile.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/banana_projectile.3ds");
   }

   else {
      fprintf(stderr, "BananaProjectile: Unrecognized environment.\n");
   }


   if (mesh) {
      mesh->resizeMesh(SVector3(1.0f));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "BananaProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3(1, 1, 1));

   if(Direction == Projectile::LEFT)
      Rotation = SVector3(-90, 0, -45);
   else if (Direction == Projectile::RIGHT)
      Rotation = SVector3(-90, 0, 45);

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileBanana::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y, 0));

   if (Direction == Projectile::LEFT) {
      Rotation.Y+=2.4f;
      SceneObject->setRotation(Rotation);
   }

   else {
      Rotation.Y-=2.4f;
      SceneObject->setRotation(Rotation);
   }
}

void CElementEnemyProjectileBanana::updatePhysicsEngineObject(float time) {
   printf("SinValue: %f\n", SinValue);

   if (Direction == Projectile::LEFT) { //go left
      PhysicsEngineObject->setVelocity(SVector2(-5.0f*cos(CosValue), 2.5f*sin(SinValue)));
   }
   
   else { //go right
      PhysicsEngineObject->setVelocity(SVector2(5.f*cos(CosValue), 2.5f*sin(SinValue)));
   }

   SinValue += time;
   CosValue += time;
}

void CElementEnemyProjectileBanana::printInformation() {
   printf("CElementEnemyProjectileBanana; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
