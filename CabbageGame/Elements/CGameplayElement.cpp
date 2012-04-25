#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable *& c, ISceneObject *& s, SRect2 a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a), Dead(true)
{
}

SRect2 & CGameplayElement::getArea() {
   return Area;
}

void CGameplayElement::update(float time) {
   ElapsedTime += time;
   if(!Dead) {
      if(Level.isLoaded()) {
         Area = PhysicsEngineObject->getArea();
         updatePhysicsEngineObject(time);
      }
      if(Level.shouldRender())
         updateSceneObject(time);
   }
}

void CGameplayElement::setupObjects() {
   if(Level.isLoaded()) {
      setupPhysicsEngineObject();
      PhysicsEngineObject->setCollisionResponder(this);
      PhysicsEngineObject->setElement(this);
      PhysicsEngineObject->getElement();
   }
   if(Level.shouldRender())
      setupSceneObject();
   Dead = false;
}

void CGameplayElement::printInformation() {
   printf("CGameplayElement; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}


CCollideable *& CGameplayElement::getPhysicsEngineObject() {
   return PhysicsEngineObject;
}

void CGameplayElement::removeFromEngines() {
   if(!Dead) {
      Level.getPhysicsEngine().remove(PhysicsEngineObject);
      CApplication::get().getSceneManager().removeSceneObject(SceneObject);
      Dead = true;
   }
}

void CGameplayElement::reactToAbility(Abilities::EAbilityType ability) {
   //Default behavior to react to abilities:
   return;
}
