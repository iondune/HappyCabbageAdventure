#include "CElementEnemyPear.h"
#include "CGameLevel.h"

CElementEnemyPear::CElementEnemyPear(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::PEAR), ISquishable(nArea.Size.X, nArea.Size.Y), OldPositionX(nArea.Position.X), HitPlayer(false), Scale(SVector2f(1.0f)) {
   printf("Made a pear!\n");
}

void CElementEnemyPear::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
   PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyPear::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
	   mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");
   else if (Level.getEnvironment() == Env::DESERT)
	   mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");
   else if (Level.getEnvironment() == Env::WATER) {
	   mesh = CMeshLoader::load3dsMesh("Base/water_pear.3ds");
   }
   else
      mesh = CMeshLoader::load3dsMesh("Base/pear.3ds");

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
   //SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   SceneObject->setScale(SVector3f(Scale.X,Scale.X,Scale.Y));
   SceneObject->setRotation(SVector3f(-90, 0, 20));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyPear::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject())
      HitPlayer = true;
   CElementEnemy::OnCollision(Event);
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyPear::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   float difference = Area.Position.X - OldPositionX;

   if (difference < .001f && difference > -.001f && !HitPlayer) {
      if (PhysicsEngineObject->getAction() == CCollisionActor::EActionType::MoveRight)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
      else
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }

   OldPositionX = Area.Position.X;
   HitPlayer = false;
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyPear::updateSceneObject(float time) {
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

void CElementEnemyPear::printInformation() {
   printf("CElementEnemyPear; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
