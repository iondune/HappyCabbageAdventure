#include "CGameplayManager.h"

#include "CGameLevel.h"
#include "CabbageScene.h"
#include "CabbageFramework.h"

#define DEBUG_PRINTFS

CGameplayManager::CGameplayManager(CGameLevel & level) {
   PhysicsEngine = new CCollisionEngine();
   level.setPhysicsEngine(PhysicsEngine);

   CCameraControl *Camera = new CCameraControl();
   Camera->setPosition(SVector3(0, 0, 20));
   Camera->setLookDirection(SVector3(0, 0, -1));
   CApplication::get().getSceneManager().setActiveCamera(Camera);

   CApplication::get().getSceneManager().Lights.clear();

   CApplication::get().getSceneManager().Lights.push_back(new CLight());
   CApplication::get().getSceneManager().Lights.back()->Color = SVector3(0.4f);
   CApplication::get().getSceneManager().Lights.back()->Position = SVector3(-5.f, 200.f, 500.f);

   //level.setNoRender();
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
   CApplication::get().getSceneManager().load();
}

void CGameplayManager::update(float time) {
   ((CCameraControl* const)CApplication::get().getSceneManager().getActiveCamera())->update(time);
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
}
