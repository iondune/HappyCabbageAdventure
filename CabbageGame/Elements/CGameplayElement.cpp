#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable *& c, ISceneObject *& s, SRect2f a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a), Dead(true), ParticleEngine(NULL)
{
}

SRect2f & CGameplayElement::getArea() {
   return Area;
}

void CGameplayElement::setArea(SRect2f r) {
   Area = r;
   PhysicsEngineObject->setArea(r);
}

void CGameplayElement::update(float time) {
   ElapsedTime += time;
   if(!Dead) {
      if(Level.isLoaded() && !ParticleEngine) {
         Area = PhysicsEngineObject->getArea();
         updatePhysicsEngineObject(time);
      }
      if(Level.shouldRender()) {
         updateSceneObject(time);
      }
   }
   if(ParticleEngine) {
      ParticleEngine->step(time);
      //ParticleEngine->setCenterPos(time);
      if(ParticleEngine->dead) {
         removeFromGame();
      }
   }
}

void CGameplayElement::setupObjects() {
   setupPhysicsEngineObject();
   if(Level.isLoaded()) {
      PhysicsEngineObject->setCollisionResponder(this);
      PhysicsEngineObject->setElement(this);
   }
   else {
      Level.getPhysicsEngine().remove(PhysicsEngineObject);
      Level.getPhysicsEngine().update(0.01f);
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

void CGameplayElement::removeFromPhysicsEngine() {
   if(!Dead) {
      Level.getPhysicsEngine().remove(PhysicsEngineObject);
   }
}

void CGameplayElement::removeFromSceneManager() {
   if(!Dead) {
      CApplication::get().getSceneManager().removeSceneObject(SceneObject);
   }
}

void CGameplayElement::removeFromGame() {
   if(!Dead) {
      removeFromPhysicsEngine();
      removeFromSceneManager();
      if(ParticleEngine) {
         ParticleEngine->deconstruct();
         delete ParticleEngine;
         ParticleEngine = NULL;
      }
      Dead = true;
   }
}

void CGameplayElement::reactToAbility(Abilities::EAbilityType ability) {
   //Default behavior to react to abilities:
   return;
}

bool CGameplayElement::isDead() {
   return Dead;
}
