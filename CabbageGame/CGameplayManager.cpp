#include "CGameplayManager.h"

#include "CGameplayElement.h"
#include "CGameLevelLoader.h"

#include "CGameLevel.h"
#include "CabbageScene.h"
#include "CabbageFramework.h"
#include "CGameplayGUIManager.h"
#include "CBiggerBlock.h"

#define DEBUG_PRINTFS

CGameplayManager::CGameplayManager(CGameLevel & level) : Level(level), Elements(level.getGameplayElements()) {
   srand((unsigned int) 52);
   PhysicsEngine = new CCollisionEngine();

   Level.setPhysicsEngine(PhysicsEngine);

   if (level.getEnvironment() == Env::WATER) {
	   PhysicsEngine->setGravity(6.25f);
      Level.getPhysicsEngine().setImpulseMultiplier(0.4);
   }

   CPerspectiveCamera *Camera = new CPerspectiveCamera(CApplication::getAspectRatio(), 0.01f, 100.0f, 60.0f);
   Camera->setPosition(SVector3f(0, 0, 20));
   Camera->setLookDirection(SVector3f(0, 0, -1));
   CApplication::get().getSceneManager().setActiveCamera(Camera);

   CApplication::get().getSceneManager().Lights.clear();

   CApplication::get().getSceneManager().Lights.push_back(new CLight());
   CApplication::get().getSceneManager().Lights.back()->Color = SVector3f(0.4f);
   CApplication::get().getSceneManager().Lights.back()->Position = SVector3f(-5.f, 200.f, 500.f);

   CApplication::get().getSceneManager().setDeferred(level.isNight());

   if (Level.getEnvironment() == Env::DESERT && ! Level.isNight())
      CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_HEAT_WAVE, true);

   if (Level.getEnvironment() == Env::WATER)
   {
      CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_WATER_DISTORT, true);
	  CApplication::get().getSceneManager().addSceneObject(new CDirectionalLightSceneObject(SVector3f(0, -1, 0), SColor(0.2f, 0.4f, 0.7f)));
	  ((CDeferredShadingManager *)CApplication::get().getSceneManager().getEffectManager())->OverlayColor = SColor(0.8f, 1.2f, 1.6f);
   }
   else
   { 
	  CApplication::get().getSceneManager().addSceneObject(new CDirectionalLightSceneObject(SVector3f(0, -1, 0), SColor(0.3f, 0.4f, 0.7f)));
     if(Level.isNight())
	      ((CDeferredShadingManager *)CApplication::get().getSceneManager().getEffectManager())->OverlayColor = SColor(1.f, 1.f, 1.f);
   }
   //level.setNoRender();
   //Set up each object
   for(unsigned int i = 0; i < level.getGameplayElements().size(); i++) {
      level.getGameplayElements()[i]->setupObjects();
   }
   //Add consolidated blocks
   if(Level.isLoaded()) {
      for(unsigned int i = 0; i < level.getConsolidatedBlocks().size(); i++) {
         level.getConsolidatedBlocks()[i]->addToEngine(PhysicsEngine);
      }
   }
#ifdef DEBUG_PRINTFS
   for(unsigned int i = 0; i < level.getGameplayElements().size(); i++) {
      printf("Element %d: ", i);
      level.getGameplayElements()[i]->printInformation();
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
      Actors[i]->getGameplayElement().printInformation();
   }
   */
   
   std::vector<CGameplayElement *> KillList;
   static float const UpdateWidth = 25.0f;
   static float const UpdateHeight = 25.0f;
   SRect2f UpdateArea = Level.getPlayer().getArea();
   UpdateArea.Position.X -= UpdateWidth/2.0f;
   UpdateArea.Position.Y -= UpdateHeight/2.0f;
   UpdateArea.Size.X = UpdateWidth;
   UpdateArea.Size.Y = UpdateHeight;

   for(unsigned int i = 0; i < Elements.size(); i++) {
      if(UpdateArea.intersects(Elements[i]->getArea()))
         Elements[i]->update(time);
      if(Elements[i]->isDead())
         KillList.push_back(Elements[i]);
   }
   GUIManager->update(time);
   for(unsigned int i = 0; i < KillList.size(); i++) {
      KillList[i]->removeFromGame();
      Level.removeCollideable(KillList[i]);
      delete KillList[i];
   }
}

void CGameplayManager::OnEnd() {
   for(unsigned int i = 0; i < Level.getElements().size(); i++) {
      Level.getElements()[i]->removeFromGame();
   }
   CApplication::get().getSceneManager().setDeferred(false);
   CApplication::get().getSceneManager().removeAllSceneObjects();
   CGameLevelLoader::resetLevel();
}
