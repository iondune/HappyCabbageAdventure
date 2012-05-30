#include "CGameState.h"
#include "CGameLevel.h"

CGameState::CGameState() {

}

#include "CSceneEffectManager.h"
void CGameState::begin() {
   Level = &CGameLevelLoader::loadLevel(LevelName);
   Level->setPlayerInformation(Stats);

   GameplayManager = new CGameplayManager(*Level);
   DecorManager = new CDecorManager(*Level);

   Level->printInformation();

   Level->setupSoundtrack();
   CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_BLOOM, true);
}

void CGameState::OnRenderStart(float const Elapsed) {
   GameplayManager->update(Elapsed);
   DecorManager->update(Elapsed);
}

void CGameState::OnRenderEnd(float const Elapsed) {
   CApplication::get().getSceneManager().drawAll();
   CApplication::get().getGUIEngine().drawAll();
   CApplication::get().getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}

void CGameState::end() {
   GameplayManager->OnEnd();
   CApplication::get().getSceneManager().setDeferred(false);
   CApplication::get().getSceneManager().removeAllSceneObjects();
   CGameLevelLoader::resetLevel();
   printf("Here in CGameState::end\n");
}

CGameLevel & CGameState::getCurrentLevel() {
   return *Level;
}

#include "../StateOverworld/COverworldState.h"
#include "../CabbageGame/Elements/Player/CElementPlayer.h"
void CGameState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   if(Event.Pressed){
      if(Event.Key == SDLK_ESCAPE) {
         Mix_FadeOutMusic(2000);
         COverworldState::get().levelCompleted = false;
         COverworldState::get().NewStats = Level->getPlayer().getStats();
         CApplication::get().getStateManager().setState(new CFadeOutState(& COverworldState::get()));

         if (Level->getEnv() == Env::DESERT)
            CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_HEAT_WAVE, false);
         if (Level->getEnv() == Env::WATER)
        	 CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_WATER_DISTORT, false);
      }
   }
   else  {
   }
}
