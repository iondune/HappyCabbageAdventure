#include "CElementEnemyLemon.h"
#include "CGameLevel.h"

CElementEnemyLemon::CElementEnemyLemon(SRect2f nArea) :
   CElementEnemy(nArea, Enemies::LEMON), ISquishable(nArea.Size.X, nArea.Size.Y) {
}

void CElementEnemyLemon::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;
   PhysicsEngineObject->getAttributes().WalkAccel = 10.f;

   //Why is this set to true?????  Something with physics stuff... and it's not calling setJumping.  : P
   //Note, being dropped from the sky in the case it's jumping.  Checked Bounce, that wasn't the problem.
   PhysicsEngineObject->setJumping(false);
}

void CElementEnemyLemon::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/lemon.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/lemon.3ds");
   }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/lemon.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3f(-90, 0, 0));

   Scale = Area.Size;
   SceneObject->setScale(SVector3f(Scale.X, Scale.X, Scale.Y));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyLemon::updatePhysicsEngineObject(float time) {
   SVector2f PlayerPosition = Level.getPlayer().getArea().Position;
   SVector2f Difference = SVector2f (PlayerPosition.X - Area.Position.X, PlayerPosition.Y - Area.Position.Y);

   if (abs(Difference.X) < 1.5f && abs(Difference.Y) < .5f) {
      Level.getPlayer().subtractHealth(2);
      takeDamage(1);
   }
   else if (abs(Difference.X) < 5.f && Difference.Y > -.2f) {
      if (Difference.X < 0.f) {
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
      }
      else if (Difference.X > 0.f) {
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
      }
   }

   else
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyLemon::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f(Area.getCenter().X,Area.getCenter().Y, 0));
   if (PhysicsEngineObject->getVelocity().X < 0.0f)
      SceneObject->setRotation(SVector3f(-90.f, (float) PhysicsEngineObject->getVelocity().X*10.0f, -30.f));
   else if (PhysicsEngineObject->getVelocity().X >= 0.0f)
      SceneObject->setRotation(SVector3f(-90.f, (float) PhysicsEngineObject->getVelocity().X*10.0f, 30.f));

   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, 0));
      SceneObject->setScale(SVector3f(Scale.X, Scale.X, 0.3f));
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

int CElementEnemyLemon::takeDamage(int amount) {
   CurHealth = std::max(0, CurHealth - amount);

   explode();

   return CurHealth;
}

void CElementEnemyLemon::explode() {
   CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
   PlayerActor->addImpulse(SVector2f(0.0f, 20.0f));

   Level.getPlayer().setShaking(1.5f, .3f);

   if (rand()%3 == 0)
      dropItem();

   dieWithSeeds();
}

void CElementEnemyLemon::printInformation() {
   printf("CElementEnemyApple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
