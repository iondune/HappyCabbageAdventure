#include "CElementEnemyProjectileGrape.h"
#include "CGameLevel.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemyProjectileGrape::CElementEnemyProjectileGrape(SRect2f nArea)
: CElementEnemyProjectile(nArea, Enemies::GRAPE_PROJECTILE) {
}


void CElementEnemyProjectileGrape::setupPhysicsEngineObject() {
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

void CElementEnemyProjectileGrape::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   int random = rand() % 3;

   if (Level.getEnvironment() == 0) {
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/grape3.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      if (random == 0)
         mesh = CMeshLoader::load3dsMesh("Base/grape1.3ds");
      else if (random == 1)
         mesh = CMeshLoader::load3dsMesh("Base/grape2.3ds");
      else
         mesh = CMeshLoader::load3dsMesh("Base/grape3.3ds");
   }

   else {
      fprintf(stderr, "GrapeProjectile: Unrecognized environment.\n");
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
   if (PlayerLeft) { //go left
      PhysicsEngineObject->setVelocity(SVector2f(-5.f, 0.f));
   }
   
   else { //go right
      PhysicsEngineObject->setVelocity(SVector2f(5.f, 0.f));
   }
}

void CElementEnemyProjectileGrape::printInformation() {
   printf("CElementEnemyProjectileGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]];\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
