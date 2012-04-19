#include "CGameplayElement.h"
#include "CGameLevel.h"

#include "CGameState.h"
CGameplayElement::CGameplayElement(CCollideable * c, ISceneObject * s) :
SceneObject(s), PhysicsEngineObject(c), Level(CGameLevelLoader::getLatestLevel())
{
}
