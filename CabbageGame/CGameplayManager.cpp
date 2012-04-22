#include "CGameplayManager.h"

#include "CGameLevel.h"

#define DEBUG_PRINTFS

CGameplayManager::CGameplayManager(CGameLevel & level) {
   PhysicsEngine = new CCollisionEngine();
   level.setPhysicsEngine(PhysicsEngine);
   level.setNoRender();
   //Set up each object
   for(int i = 0; i < level.getElements().size(); i++) {
      level.getElements()[i]->setupObjects();
#ifdef DEBUG_PRINTFS
      printf("Setting up object #%d:\n", i);
      level.getElements()[i]->printInformation();
#endif
   }
   //Add consolidated blocks
   for(int i = 0; i < level.getConsolidatedBlocks().size(); i++) {
      level.getConsolidatedBlocks()[i]->addToEngine(PhysicsEngine);
   }
#ifdef DEBUG_PRINTFS
   printf("Total number in physics engine after setup: %d (%d actors + %d objects).\n",
      PhysicsEngine->getActors().size() + PhysicsEngine->getObjects().size(),
      PhysicsEngine->getActors().size(), PhysicsEngine->getObjects().size());
#endif
}

void CGameplayManager::update(float time) {
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
