#include "CElementEnemy.h"
#include "CGameLevel.h"

#include "CElementEnemyApple.h"
#include "CElementEnemyBlade.h"
#include "CElementEnemyOrange.h"
#include "CElementEnemyGrape.h"
#include "CElementEnemyKiwi.h"
#include "CElementEnemyFlame.h"

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
      Level.getPlayer().decrementHealth();
      printf("Health is now %d\n", Level.getPlayer().getStats().Health);
      Level.removeEnemy(this);
      removeFromEngines();
      Dead = true;
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
