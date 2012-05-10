#include "CElementEnemyStrawberry.h"
#include "CGameLevel.h"

CElementEnemyStrawberry::CElementEnemyStrawberry(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::STRAWBERRY), ISquishable(nArea.Size.X, nArea.Size.Y) {

}

void CElementEnemyStrawberry::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 4.5f;
   PhysicsEngineObject->getAttributes().WalkAccel = 10.0f;
   //PhysicsEngineObject->getAttributes().JumpLength = .5f;
}

void CElementEnemyStrawberry::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/Strawberry.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/Strawberry.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/Strawberry.3ds");

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
   SceneObject->setRotation(SVector3f(-90, 0, -30));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyStrawberry::updatePhysicsEngineObject(float time) {
   SVector2 PlayerPosition = Level.getPlayer().getArea().Position;

   if (PlayerPosition.X < Area.getCenter().X - 3.f)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (PlayerPosition.X > Area.getCenter().X + 3.f)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   else {
      PhysicsEngineObject->setJumping(true);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyStrawberry::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(1.0f, 1.0f, 0.3f));
      return;
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3f(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3f(-1,1,1));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3f(1,1,1));
   }
}

void CElementEnemyStrawberry::printInformation() {
   printf("CElementEnemyStrawberry; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
