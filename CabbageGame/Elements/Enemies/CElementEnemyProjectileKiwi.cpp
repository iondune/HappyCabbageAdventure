#include "CElementEnemyProjectileKiwi.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileKiwi::CElementEnemyProjectileKiwi(SRect2f nArea)
: CElementEnemyProjectile(nArea, Enemies::KIWI_PROJECTILE) {

}

void CElementEnemyProjectileKiwi::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   //Set actor attributes
   PhysicsEngineObject->setGravityEnabled(false);

   PhysicsEngineObject->getAttributes().MaxWalk = 4.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;

   PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);

   if (Level.getEnv() != Env::WATER)
      PhysicsEngineObject->setVelocity(SVector2f(0.f, -6.f));
   else
      PhysicsEngineObject->setVelocity(SVector2f(0.f, -1.f));

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyProjectileKiwi::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
	   mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");

   else if (Level.getEnvironment() == Env::DESERT)
	   mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");

   else if (Level.getEnvironment() == Env::WATER) {
	   mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");
   }

   else
      mesh = CMeshLoader::load3dsMesh("Base/kiwi_seed.3ds");

   if (mesh) {
      mesh->resizeMesh(SVector3f(.4f));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      fprintf(stderr, "KiwiProjectile:  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setScale(SVector3f(1, 1, 1));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyProjectileKiwi::OnCollision(CCollideable *Object) {
   if (!Dead) {
      removeFromGame();
      Dead = true;

      if (Object == Level.getPlayer().getPhysicsEngineObject()) {
         CCollisionActor *PlayerActor = ((CCollisionActor *)Level.getPlayer().getPhysicsEngineObject());

         if (Level.getPlayer().decrementHealth()) {

           if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
              PlayerActor->addImpulse(SVector2f(4.f, 2.f));

           else
              PlayerActor->addImpulse(SVector2f(-4.f, 2.f));

           Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
   }
}

void CElementEnemyProjectileKiwi::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   PhysicsEngineObject->setVelocity(SVector2f(0.f, -6.f));
}

void CElementEnemyProjectileKiwi::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));
}

void CElementEnemyProjectileKiwi::printInformation() {
   printf("CElementEnemyProjectileKiwi; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
