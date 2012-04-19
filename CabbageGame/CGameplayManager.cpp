#include "CGameplayManager.h"

#include "CGameLevel.h"

CGameplayManager::CGameplayManager(CGameLevel & level) {
   PhysicsEngine = new CCollisionEngine();
   for(int i = 0; i < level.getElements().size(); i++) {
      level.getElements()[i]->setupObjects();
   }
}

void CGameplayManager::update(float time) {
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
