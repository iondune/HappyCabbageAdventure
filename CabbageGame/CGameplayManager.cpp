#include "CGameplayManager.h"

#include "CGameLevel.h"
#include "CabbageScene.h"
#include "CabbageFramework.h"

#define DEBUG_PRINTFS

CGameplayManager::CGameplayManager(CGameLevel & level) : Level(level), Elements(level.getElements()) {
   PhysicsEngine = new CCollisionEngine();
   level.setPhysicsEngine(PhysicsEngine);

   CPerspectiveCamera *Camera = new CPerspectiveCamera(CApplication::getAspectRatio(), 0.01f, 100.0f, 60.0f);
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
   }
   //Add consolidated blocks
   for(int i = 0; i < level.getConsolidatedBlocks().size(); i++) {
      level.getConsolidatedBlocks()[i]->addToEngine(PhysicsEngine);
   }
#ifdef DEBUG_PRINTFS
   for(int i = 0; i < level.getElements().size(); i++) {
      printf("Element %d: ", i);
      level.getElements()[i]->printInformation();
   }
   printf("Total number in physics engine after setup: %d (%d actors + %d objects).\n",
      PhysicsEngine->getActors().size() + PhysicsEngine->getObjects().size(),
      PhysicsEngine->getActors().size(), PhysicsEngine->getObjects().size());
#endif

   GUIManager = new CGameplayGUIManager(level.getPlayer().getStats());

   CApplication::get().getSceneManager().load();
}

void CGameplayManager::update(float time) {
   //CApplication::get().getSceneManager().getActiveCamera()->update(time);
   PhysicsEngine->update(time);
   for(int i = 0; i < Elements.size(); i++) {
      Elements[i]->update(time);
   }
   GUIManager->update();
}
