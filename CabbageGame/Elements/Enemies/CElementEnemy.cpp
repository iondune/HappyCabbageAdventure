#include "CElementEnemy.h"
#include "CGameLevel.h"

#include "CElementEnemyApple.h"
#include "CElementEnemyBlade.h"
#include "CElementEnemyOrange.h"
#include "CElementEnemyGrape.h"
#include "CElementEnemyKiwi.h"
#include "CElementEnemyFlame.h"

#include "CElementItemSeed.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemy::CElementEnemy(SRect2 nArea, Enemies::EEnemyType type)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), PhysicsEngineObject(NULL), SceneObject(NULL), Type(type) {
}

//Enemy created by factory
CElementEnemy *CEnemyLoader::LoadEnemy(SRect2 nArea, Enemies::EEnemyType type) {
   switch(type) {
   case Enemies::APPLE:
      return new CElementEnemyApple(nArea);
      break;
   case Enemies::ORANGE:
      return new CElementEnemyOrange(nArea);
      break;
   case Enemies::KIWI:
      return new CElementEnemyKiwi(nArea, 0);
      break;
   case Enemies::GRAPE:
      return new CElementEnemyGrape(nArea);
      break;
   case Enemies::FLAME:
      return new CElementEnemyFlame(nArea);
      break;
   case Enemies::BLADE:
      return new CElementEnemyBlade(nArea);
      break;
   case Enemies::KIWI_PROJECTILE:
      return NULL;//new CElementEnemyKiwiProjectile(nArea);
      break;
   case Enemies::GRAPE_PROJECTILE:
      return NULL;//new CElementEnemyGrapeProjectile(nArea);
      break;
   default:
      fprintf(stderr, "Error: unknown enemy type %d\n", type);
      exit(1);
      break;
   }
}

void CElementEnemy::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, eType;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    widthValue << Area.Size.X;
    heightValue << Area.Size.Y;
    tagValue << "CEnemy";
    eType << Type;
    //put code for type
    // 1: apple 2: orange
    l->AddAtributes("type ", eType.str());
    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}

void CElementEnemy::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();
      if(Level.getPlayer().getArea().Position.Y > Area.otherCorner().Y) {
         Level.removeEnemy(this);
         removeFromEngines();
         Dead = true;
         Area.Position.Y += 0.3f;
         for(int i = 0; i < rand()%7 + 3; i++) {
            CElementItem *seed;
            Level.addItem(seed = CItemLoader::LoadItem(Area, Items::SEED));

            float rand1 = (float)rand()/(float)RAND_MAX;
            float rand2 = (float)rand()/(float)RAND_MAX;

            ((CCollisionActor *)seed->getPhysicsEngineObject())->setImpulse(SVector2(rand1*8.f - 4.f, rand2*4.5f + 1.0f), 0.01f);
         }
         PlayerActor->setImpulse(SVector2(0.0f, 3.0f), 0.01f);
         Level.getPlayer().setShaking(0.4f, 3.0f);
      }
      else {
         if(Level.getPlayer().decrementHealth()) {
            if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
               PlayerActor->setImpulse(SVector2(7.f, 2.8f), 0.1f);
            else
               PlayerActor->setImpulse(SVector2(-7.f, 2.8f), 0.1f);
            Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
   }
}

void CElementEnemy::updatePhysicsEngineObject(float time) {
   fprintf(stderr, "Error: updatePhysicsEngineObject on generic enemy type %d (perhaps the CElementEnemy::updatePhysicsEngineObject function wasn't overridden?).\n", Type);
   exit(1);
}

void CElementEnemy::updateSceneObject(float time) {
   return;
}

Enemies::EEnemyType CElementEnemy::getEnemyType() {
   return Type;
}

void CElementEnemy::printInformation() {
   printf("CElementEnemy; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Type);
}
