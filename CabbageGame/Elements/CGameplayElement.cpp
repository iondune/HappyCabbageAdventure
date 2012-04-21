#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable * c, ISceneObject * s, SRect2 a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a)
{
}

void CGameplayElement::update(float time) {
   ElapsedTime += time;
   if(Level.isLoaded())
      updatePhysicsEngineObject(time);
   updateSceneObject(time);
}

void CGameplayElement::setupObjects() {
   if(Level.isLoaded())
      setupPhysicsEngineObject();
   setupSceneObject();
}
