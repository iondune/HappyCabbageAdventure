#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable *& c, ISceneObject *& s, SRect2 a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a)
{
}

SRect2 & CGameplayElement::getArea() {
   return Area;
}

void CGameplayElement::update(float time) {
   ElapsedTime += time;
   if(Level.isLoaded()) {
      Area = PhysicsEngineObject->getArea();
      updatePhysicsEngineObject(time);
   }
   if(Level.shouldRender())
      updateSceneObject(time);
}

void CGameplayElement::setupObjects() {
   if(Level.isLoaded()) {
      setupPhysicsEngineObject();
      PhysicsEngineObject->setCollisionResponder(this);
   }
   if(Level.shouldRender())
      setupSceneObject();
}

void CGameplayElement::printInformation() {
   printf("CGameplayElement; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
