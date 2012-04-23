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

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y+1))/2, 0));
   SceneObject->setScale(SVector3(1, 1, 1));
   SceneObject->setRotation(SVector3(-90, 0, 90));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

/*
void CElementEnemyGrape::OnCollision(CCollideable *Object) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
*/
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyGrape::updatePhysicsEngineObject(float time) {
   //TODO: Make some class singleton so we can get the player's location
/*   if (Manager->getPlayerLocation().X < Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (Manager->getPlayerLocation().X > Area.getCenter().X && (Roll == None))
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
*/
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
   shootTime += time;

   //TODO:  Determine if the grape should be facing left or right.
   /*if (Manager->getPlayerLocation().X < Actor->getArea().getCenter().X)
      SceneObject->setRotation(SVector3(-90, 0, 90));
   else
      SceneObject->setRotation(SVector3(-90, 0, -90));*/

   //TODO:  Determine the player is alive.
   if (shootTime - 2.f > 0.f /*&& Manager->isPlayerAlive()*/) {
      shootTime = 0.f;

      //Fire Grape Projectile
      ShootGrape();
   }
}

void CElementEnemyGrape::printInformation() {
   printf("CElementEnemyGrape; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyGrape::ShootGrape() {
   SVector2 pos = Area.Position;

   //TODO: Compare player's position to grape's position to determine what direction to shoot the grape.
   if (true/*Manager->getPlayerLocation().X < Actor->getArea().getCenter().X*/) //spawn to the left
      //TODO:  Shoot projectile grape.  Need to determine if that will be an enemy or if a separate projectile abstract class will be made.
      //CBadGuy::makeBadGuy(pos.X - w/2.f - .05f, pos.Y, w, h, pGrape, Manager, 0);
      true;
   else //spawn to the right
      //TODO: See above
      true;
}
