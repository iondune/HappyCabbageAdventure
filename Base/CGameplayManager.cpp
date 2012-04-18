#include "CGameplayManager.h"

void CGameplayManager::update(float time) {
   PhysicsEngine->update(Time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
