#include "CGameplayElement.h"
#include "CGameLevel.h"

CGameplayElement::CGameplayElement(CCollideable * c, ISceneObject * s, SRect2 a) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel()), Area(a)
{
}
