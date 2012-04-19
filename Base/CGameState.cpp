#include "CGameState.h"

void CGameState::begin() {
   Level = &CGameLevelLoader::loadLevel("jorge1.xml");
   GameplayManager = new CGameplayManager(*Level);
   DecorManager = new CDecorManager(*Level);
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

CGameLevel & CGameState::getCurrentLevel() {
   return *Level;
}
