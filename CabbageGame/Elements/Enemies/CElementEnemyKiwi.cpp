#include "CElementEnemyKiwi.h"
#include "CGameLevel.h"

CElementEnemyKiwi::CElementEnemyKiwi(SRect2f nArea, int direction) :
   CElementEnemy(nArea, Enemies::KIWI), Direction(direction), rotateBird(0.0f), SineValue(0.0f), OldX(Area.Position.X), bombDropped(false) {

}

void CElementEnemyKiwi::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();

   PhysicsEngineObject->setControlFall(false);
   PhysicsEngineObject->setGravity(0.0f);

   PhysicsEngineObject->getAttributes().MaxWalk = 3.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;
   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;
   PhysicsEngineObject->getAttributes().Reacts = 0;

   CElementEnemy::setupPhysicsEngineObject();
}

void CElementEnemyKiwi::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == Env::FOREST) {
      mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
   }

   else if (Level.getEnvironment() == Env::DESERT) {
      mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
   }
   else if (Level.getEnvironment() == Env::WATER) {
         mesh = CMeshLoader::load3dsMesh("Base/water_killerkiwi.3ds");
      }
   //LevelEditor has no environment
   else
      mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");

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
   SceneObject->setScale(SVector3f(1, 1, 1));
   SceneObject->setRotation(SVector3f(-90, 0, -90));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

//This is where the AI would be updated for more complex enemies
void CElementEnemyKiwi::updatePhysicsEngineObject(float time) {
   CElementEnemy::updatePhysicsEngineObject(time);
   if(TimeToDeath > 0.0f)
      return;
   //TODO: Make some class singleton so we can get the player's location
   //TODO:  Clean up KIWI z-axis code
   if (!Level.getPlayer().isDead())
   {
      /*if(zTimer < 0.0f)
         zTimer = 0.0f;
      if(inZ) {
         if(zTimer >= Z_SPEED)
            zTimer = Z_SPEED;
         else {
            zTimer += time;
         }
      }
      else {
         if(zTimer > 0.0f)
            zTimer -= time;
         else {
         }
      }*/

      SineValue = 0.6f*sin(Area.Position.X - OldX);
      Area.Position.Y += SineValue;

      SVector2f vel = PhysicsEngineObject->getVelocity();
      PhysicsEngineObject->setVelocity(SVector2f(vel.X, vel.Y > 0 ? vel.Y - 1.0f*time : 0));

      //TODO: Check player direction
      if(Direction == 0)
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
      else
         PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);

      //TODO:  Z-Code
      //TODO:  Player location
      /*if(!inZ) {*/

      float xDist = Area.Position.X - Level.getPlayer().getArea().Position.X;

      //Drop bomb projectile
      if (xDist < .2f && !bombDropped && Direction == 0) {
         DropBomb();
         bombDropped = true;
      }
      else if (xDist > -.2f && !bombDropped && Direction == 1) {
         DropBomb();
         bombDropped = true;
      }
   }

   else
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
}

//This is where the renderable would be updated for the more complex enemies
void CElementEnemyKiwi::updateSceneObject(float time) {
   rotateBird = -100.0f * SineValue;
   
   SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));
   SceneObject->setRotation(SVector3f(-90 + rotateBird, 0, -90));

   //TODO:  Adjust Z-Collision?
   //SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, zTimer*0.9f*Depth*(1.0f/Z_SPEED)));

   if(PhysicsEngineObject->getVelocity().X < -0.01f)
      //TODO: SEE ABOVE
      //SceneObject->setScale(SVector3f(-1,1,1)*(zTimer + 1.0f));
      true;
   else if(PhysicsEngineObject->getVelocity().X > 0.01f)
      //TODO:  SEE ABOVE
      true;
      //SceneObject->setScale(SVector3f(1,1,1)*(zTimer + 1.0f));

   if(ParticleEngine) {
      SceneObject->setTranslation(SVector3f(Area.getCenter().X, Area.Position.Y, 0));
      SceneObject->setRotation(SVector3f(-90, 0, -90));
      SceneObject->setScale(SVector3f(Area.Size.X, Area.Size.X, 0.3f));
      return;
   }
   if(TimeToDeath > 0.0f) {
      CElementEnemy::updateSceneObject(time);
      return;
   }
}

void CElementEnemyKiwi::printInformation() {
   printf("CElementEnemyKiwi; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyKiwi::DropBomb() {

   float xLocation = Area.Position.X + Area.Size.X - .05f;
   float yLocation = Area.getCenter().Y - Area.Size.Y - .5f;

   Level.addEnemy(CEnemyLoader::LoadEnemy(SRect2f(xLocation, yLocation, .2f, .2f), Enemies::KIWI_PROJECTILE));
}

