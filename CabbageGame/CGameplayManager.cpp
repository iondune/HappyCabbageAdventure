#include "CGameplayManager.h"

#include "CGameLevel.h"
#include "CabbageScene.h"
#include "CabbageFramework.h"
#include "CGameplayGUIManager.h"

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

   CApplication::get().getSceneManager().setDeferred(level.isNight());
   //level.setNoRender();
   //Set up each object
   for(int i = 0; i < level.getElements().size(); i++) {
      level.getElements()[i]->setupObjects();
   }
   //Add consolidated blocks
   if(Level.isLoaded()) {
      for(int i = 0; i < level.getConsolidatedBlocks().size(); i++) {
         level.getConsolidatedBlocks()[i]->addToEngine(PhysicsEngine);
      }
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
   if(Level.isLoaded())
      PhysicsEngine->update(time);
   /*
   std::vector<CCollisionActor*> Actors = PhysicsEngine->getActors();
   for(int i = 0; i < Actors.size(); i++) {
      Actors[i]->getElement().printInformation();
   }
   */
   
   std::vector<CGameplayElement *> KillList;
   static float const UpdateWidth = 30.0f;
   static float const UpdateHeight = 30.0f;
   SRect2 UpdateArea = Level.getPlayer().getArea();
   UpdateArea.Position.X -= UpdateWidth/2.0f;
   UpdateArea.Position.Y -= UpdateHeight/2.0f;
   UpdateArea.Size.X = UpdateWidth;
   UpdateArea.Size.Y = UpdateHeight;

   for(int i = 0; i < Elements.size(); i++) {
      if(UpdateArea.intersects(Elements[i]->getArea()))
         Elements[i]->update(time);
      if(Elements[i]->isDead())
         KillList.push_back(Elements[i]);
   }
   GUIManager->update(time);
   for(int i = 0; i < KillList.size(); i++) {
      KillList[i]->removeFromGame();
      Level.removeObject(KillList[i]);
      delete KillList[i];
   }
}
