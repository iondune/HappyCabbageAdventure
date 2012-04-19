#include "CGameplayManager.h"

CGameplayManager::CGameplayManager(CGameLevel & level) {
   PhysicsEngine = new CEngine();
   for(int i = 0; i < level.getPlaceables().size(); i++) {
      level.getPlaceables()[i]->setup();
   }
}

void CGameplayManager::update(float time) {
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
