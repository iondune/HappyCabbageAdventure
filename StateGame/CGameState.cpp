#include "CGameState.h"
#include "CGameLevel.h"

CGameState::CGameState() {

}

void CGameState::begin() {
   printf("Here in begin\n");
   Level = &CGameLevelLoader::loadLevel(LevelName);
   Level->setPlayerInformation(Stats);

   GameplayManager = new CGameplayManager(*Level);
   DecorManager = new CDecorManager(*Level);

   Level->printInformation();

   Level->setupSoundtrack();
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
   CApplication::get().getSceneManager().setDeferred(false);
   CApplication::get().getSceneManager().removeAllSceneObjects();
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
      }
   }
   else  {
   }
}
