#include "CGameplayManager.h"

#include "CGameLevel.h"

#define DEBUG_PRINTFS

CGameplayManager::CGameplayManager(CGameLevel & level) {
   PhysicsEngine = new CCollisionEngine();
   level.setNoRender();
   for(int i = 0; i < level.getElements().size(); i++) {
      level.getElements()[i]->setupObjects();
#ifdef DEBUG_PRINTFS
      printf("Setting up object #%d:\n", i);
      level.getElements()[i]->printInformation();
#endif
   }
}

void CGameplayManager::update(float time) {
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
