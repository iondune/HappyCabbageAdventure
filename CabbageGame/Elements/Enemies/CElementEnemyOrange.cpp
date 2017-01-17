#include "CElementEnemyOrange.h"
#include "CGameLevel.h"

CElementEnemyOrange::CElementEnemyOrange(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::ORANGE), Jumped(true), ISquishable(1.0f, 1.0f) {

}

void CElementEnemyOrange::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   //Set actor attributes
   PhysicsEngineObject->getActorAttributes().MaxWalk = 5.0f;
   PhysicsEngineObject->getActorAttributes().WalkAccel = 4.0f;
   PhysicsEngineObject->getActorAttributes().JumpAccel = 6.4f;
   PhysicsEngineObject->getActorAttributes().JumpLength = .1f;
   PhysicsEngineObject->getActorAttributes().AirControl = 0.75f;
   PhysicsEngineObject->getActorAttributes().AirSpeedFactor = 1.0f;


   CElementEnemy::setupPhysicsEngineObject();
}
void CElementEnemyOrange::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
      mesh = CMeshLoader::load3dsMesh("Base/orange.3ds");

   else if (Level.getEnvironment() == Env::DESERT)
      mesh = CMeshLoader::load3dsMesh("Base/desertorange.3ds");
   else if (Level.getEnvironment() == Env::WATER)
	   mesh = CMeshLoader::load3dsMesh("Base/water_orange.3ds");
   else
      mesh = CMeshLoader::load3dsMesh("Base/orange.3ds");

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

//This is where the AI would be updated for more complex enemies
void CElementEnemyOrange::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   SVector2f PlayerPosition = Level.getPlayer().getArea().Position;
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

      else
    	  PhysicsEngineObject->setJumping(false);
   }
   else
      Jumped = true;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyOrange::updateSceneObject(float time) {
   if (PhysicsEngineObject->getVelocity().X < 0.0f)
      SceneObject->setRotation(SVector3f(-90, (float) PhysicsEngineObject->getVelocity().X*10.0f, -90));
   else if (PhysicsEngineObject->getVelocity().X >= 0.0f)
      SceneObject->setRotation(SVector3f(-90, (float) PhysicsEngineObject->getVelocity().X*10.0f, 90));
   else {
      /*
      if (PlayerPosition.X < Area.getCenter().X)
         SceneObject->setRotation(SVector3f(-90, 0, -90));
      else
         SceneObject->setRotation(SVector3f(-90, 0, 90));
      */
   }

   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
      return;
   }
   if(TimeToDeath > 0.0f) {
      CElementEnemy::updateSceneObject(time);
      return;
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   if(PhysicsEngineObject->getVelocity().X < -0.01f)
      SceneObject->setScale(SVector3f(-Scale.X,Scale.X,Scale.Y));
   else if(PhysicsEngineObject->getVelocity().X > 0.01f)
      SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   else
      SceneObject->setScale(SVector3f(1.0f));
}

void CElementEnemyOrange::printInformation() {
   printf("CElementEnemyOrange; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
