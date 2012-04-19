#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable * c, ISceneObject * s, SRect2 a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a)
{
}

void CGameplayElement::update(float e) {
   fprintf(stderr, "Tried to update a generic CGameplayElement!\n");
   exit(1);
}
