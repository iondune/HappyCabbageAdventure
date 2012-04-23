#include "CElementEnemyKiwi.h"
#include "CGameLevel.h"

CElementEnemyKiwi::CElementEnemyKiwi(SRect2 nArea, int direction) :
   CElementEnemy(nArea, Enemies::KIWI), Direction(direction), rotateBird(0.0f), SineValue(0.0f), OldX(Area.Position.X), bombDropped(false) {

}

void CElementEnemyKiwi::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_KIWI;
   PhysicsEngineObject->getAttributes().MaxWalk = 3.0f;
   PhysicsEngineObject->getAttributes().WalkAccel = 20.0f;

   PhysicsEngineObject->getAttributes().AirControl = 1.0f;
   PhysicsEngineObject->getAttributes().AirSpeedFactor = 1.0f;

   //TODO:  Is this still needed?
   PhysicsEngineObject->getAttributes().Reacts = 0;

   //TODO:  What should be done with:
   /*Actor->CollideableLevel = INTERACTOR_SUPERNONCOLLIDERS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;*/
}

void CElementEnemyKiwi::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   CMesh *mesh;

   if (Level.getEnvironment() == 0) {
      mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
   }

   else if (Level.getEnvironment() == 1) {
      mesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
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
   SceneObject->setTranslation(SVector3((Area.Position.X+(Area.Position.X+1))/2, (Area.Position.Y+(Area.Position.Y-1))/2, 0));
   SceneObject->setScale(SVector3(1, 1, 1));
   SceneObject->setRotation(SVector3(-90, 0, -90));

   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementEnemyKiwi::OnCollision(CCollideable *Object, CCollideable *With) {
   //Optional code: setImpulse to other object away from this object, lower their health?
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.

//This is where the AI would be updated for more complex enemies
void CElementEnemyKiwi::updatePhysicsEngineObject(float time) {
   //TODO: Make some class singleton so we can get the player's location
   //TODO:  Clean up KIWI z-axis code
   if (true/*Manager->isPlayerAlive()*/)
   {
   /*   if(zTimer < 0.0f)
         zTimer = 0.0f;
      if(inZ) {
         if(zTimer >= Z_SPEED)
            zTimer = Z_SPEED;
         else {
            zTimer += TickTime;
         }
      }
      else {
         if(zTimer > 0.0f)
            zTimer -= TickTime;
         else {
         }
      }*/

      Area.Position.Y -= SineValue;
      SineValue = 0.6f*sin(Area.Position.X - OldX);
      Area.Position.Y += SineValue;

      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);

      SVector2 vel = PhysicsEngineObject->getVelocity();
      PhysicsEngineObject->setVelocity(SVector2(vel.X, vel.Y > 0 ? vel.Y - 1.0f*time : 0));

      //TODO: Check player direction
      if(true/*Direction == 0*/)
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

   SceneObject->setRotation(SVector3(-90 + rotateBird, 0, -90));

   //TODO:  Adjust Z-Collision?
   //SceneObject->setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y, zTimer*0.9f*Depth*(1.0f/Z_SPEED)));

   if(PhysicsEngineObject->getVelocity().X < -0.01f)
      //TODO: SEE ABOVE
      //SceneObject->setScale(SVector3(-1,1,1)*(zTimer + 1.0f));
      true;
   else if(PhysicsEngineObject->getVelocity().X > 0.01f)
      //TODO:  SEE ABOVE
      true;
      //SceneObject->setScale(SVector3(1,1,1)*(zTimer + 1.0f));
}

void CElementEnemyKiwi::printInformation() {
   printf("CElementEnemyKiwi; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}

void CElementEnemyKiwi::DropBomb() {
   SVector2 pos = Area.Position;

   //TODO:  Need to determine if projectiles are an enemy or a separate abstract class.
   //TODO:  Determine the direction of the player with respect to the Kiwi.
   /*if (Direction == 0)
      CBadGuy::makeBadGuy(pos.X + w/2.f - .05f, pos.Y - .5f, w, h, pKiwi, Manager, 0);
   else
      CBadGuy::makeBadGuy(pos.X - w/2.f - .05f, pos.Y - .5f, w, h, pKiwi, Manager, 0);*/
}
