#include "CGameState.h"

void CGameState::begin() {
   CGameLevel & level = CGameLevelLoader::loadLevel("jorge1.xml");
   GameplayManager = new CGameplayManager(level);
   DecorManager = new CDecorManager(level);
}

void CGameState::OnRenderStart(float const Elapsed) {
   GameplayManager->update(Elapsed);
   DecorManager->update(Elapsed);
}

void CGameState::OnRenderEnd(float const Elapsed) {
   CApplication::get().getSceneManager().drawAll();
   CApplication::get().getGUIEngine().drawAll();
}

void CGameState::end() {

}
