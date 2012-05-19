#include "CElementEnemyGiantApple.h"
#include "CGameLevel.h"

CElementEnemyGiantApple::CElementEnemyGiantApple(SRect2 nArea) :
   CElementEnemy(nArea, Enemies::GIANT_APPLE), ISquishable(nArea.Size.X, nArea.Size.Y) {

   MaxHealth = 3;
   CurHealth = MaxHealth;
}

void CElementEnemyGiantApple::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->getAttributes().MaxWalk = 2.2f;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyGiantApple::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST)
      mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   else if (Level.getEnvironment() == Env::DESERT)
      mesh = CMeshLoader::load3dsMesh("Base/desertapple.3ds");
   else if (Level.getEnvironment() == Env::WATER)
      mesh = CMeshLoader::load3dsMesh("Base/water_apple.3ds");
   else
      mesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");

   if(mesh) {
      mesh->resizeMesh(SVector3(1));
      mesh->centerMeshByExtents(SVector3(0));
      mesh->calculateNormalsPerFace();
   }

   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   SceneObject->setMesh(mesh);
   SceneObject->setRotation(SVector3(-90, 0, 0));
   SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));

   Scale = Area.Size;
   SceneObject->setScale(SVector3(Scale.X, Scale.X, Scale.Y));
   SceneObject->setShader(ERP_DEFAULT, "Toon");
   SceneObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyGiantApple::updatePhysicsEngineObject(float time) {
   SVector2 PlayerPosition = Level.getPlayer().getArea().Position;
   //TODO: Make some class singleton so we can get the player's location
   if (PlayerPosition.X < Area.getCenter().X)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   else if (PlayerPosition.X > Area.getCenter().X)
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyGiantApple::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3(Area.getCenter().X,Area.getCenter().Y, 0));
   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3(-90, 0, 0));
      SceneObject->setScale(SVector3(Scale.X, Scale.X, 0.3f));
      return;
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   if (ScaleMult > 1.1f)
      PositiveScale = false;
   else if (ScaleMult < .9f)
      PositiveScale = true;

   if (PositiveScale)
      ScaleMult += .35f * time;
   else
      ScaleMult -= .35f * time;

   ScaleMult = std::max(std::min(ScaleMult, 1.2f), 0.8f);

   Scale.Y += ScaleMult - 1.0f;

   if (PhysicsEngineObject->getVelocity().Y < .01f && PhysicsEngineObject->getVelocity().Y > -.01f) {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(Scale.X,Scale.X,Scale.Y));
   }
   else {
      if(PhysicsEngineObject->getVelocity().X < -0.01f)
         SceneObject->setScale(SVector3(-Scale.X,Scale.X,Scale.Y));
      else if(PhysicsEngineObject->getVelocity().X > 0.01f)
         SceneObject->setScale(SVector3(Scale.X,Scale.X,Scale.Y));
   }
}

void CElementEnemyGiantApple::printInformation() {
   printf("CElementEnemyApple; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
