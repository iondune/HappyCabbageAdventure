#include "CElementEnemyOrange.h"
#include "CGameLevel.h"

CElementEnemyOrange::CElementEnemyOrange(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::ORANGE), Jumped(true) {

}

void CElementEnemyOrange::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 5.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 4.0f;
   PhysicsEngineObject->getAttributes().JumpAccel = 6.4f;
   PhysicsEngineObject->getAttributes().JumpLength = .1f;
   PhysicsEngineObject->getAttributes().AirControl = 0.75f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_ORANGE;
}

void CElementEnemyOrange::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0)
      mesh = CMeshLoader::load3dsMesh("Base/orange.3ds");
   else if (Level.getEnvironment() == 1)
      mesh = CMeshLoader::load3dsMesh("Base/desertorange.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setTexture("Base/orange.bmp");
   
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->setTranslation(SVector3((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y-1))/2, 0));
   SceneObject->setScale(SVector3(1, 1, 1));
   SceneObject->setRotation(SVector3(-90, 0, 0));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

/*
void CElementEnemyOrange::OnCollision(CCollideable *Object) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
*/
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyOrange::updatePhysicsEngineObject(float time) {
   SVector2 PlayerPosition = Level.getPlayer().getArea().Position;
   if (PlayerPosition.X < Area.getCenter().X)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

   if (PlayerPosition.X - Area.getCenter().X < 2.2 &&
         PlayerPosition.X - Area.getCenter().X > -2.2) {
      if (Jumped) {
         PhysicsEngineObject->setJumping(true);
         Jumped = false;
      }
   }
   else
      Jumped = true;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyOrange::updateSceneObject(float time) {
   if (PhysicsEngineObject->getVelocity().X < 0.0f)
      SceneObject->setRotation(SVector3(-90, PhysicsEngineObject->getVelocity().X*10.0f, -90));
   else if (PhysicsEngineObject->getVelocity().X >= 0.0f)
      SceneObject->setRotation(SVector3(-90, PhysicsEngineObject->getVelocity().X*10.0f, 90));
   else {
      /*
      if (PlayerPosition.X < Area.getCenter().X)
         SceneObject->setRotation(SVector3(-90, 0, -90));
      else
         SceneObject->setRotation(SVector3(-90, 0, 90));
      */
   }

   SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));

   if(PhysicsEngineObject->getVelocity().X < -0.01f)
      SceneObject->setScale(SVector3(-1,1,1));
   else if(PhysicsEngineObject->getVelocity().X > 0.01f)
      SceneObject->setScale(SVector3(1,1,1));
}

void CElementEnemyOrange::printInformation() {
   printf("CElementEnemyOrange; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
