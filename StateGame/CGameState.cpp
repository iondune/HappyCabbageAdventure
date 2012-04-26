#include "CGameState.h"
#include "CGameLevel.h"

CGameState::CGameState() {

}

void CGameState::begin() {
   printf("Here in begin\n");
   Level = &CGameLevelLoader::loadLevel(LevelName);

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

}

CGameLevel & CGameState::getCurrentLevel() {
   return *Level;
}
