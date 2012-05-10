#include "CElementEnemyCherry.h"
#include "CElementEnemyProjectileCherry.h"
#include "CGameLevel.h"

CElementEnemyCherry::CElementEnemyCherry(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::CHERRY), BombTime(0.0f) {

}

void CElementEnemyCherry::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_CHERRY;
}

void CElementEnemyCherry::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/cherries.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/cherries.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/cherries.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y-1))/2, 0));
   SceneObject->setScale(SVector3f(Area.Size.X, Area.Size.X, Area.Size.Y));
   SceneObject->setRotation(SVector3f(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyCherry::updatePhysicsEngineObject(float time) {
   if (BombTime >= 3.0f) {
         BombTime = 0.0f;

         SRect2f projectileArea = Area;
         projectileArea.Position.Y += .3f;
         projectileArea.Position.X -= 1.0f;

         Level.addEnemy(new CElementEnemyProjectileCherry(projectileArea, -2.0f));

         projectileArea.Position.X += 2.0f;
         Level.addEnemy(new CElementEnemyProjectileCherry(projectileArea, 2.0f));
      }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyCherry::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
      return;
   }

   BombTime += time;
}

void CElementEnemyCherry::printInformation() {
   printf("CElementEnemyCherry; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
