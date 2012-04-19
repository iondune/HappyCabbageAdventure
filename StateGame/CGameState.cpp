#include "CGameState.h"

CGameState::CGameState() {

}

void CGameState::begin() {
   printf("Here in begin\n");
   Level = &CGameLevelLoader::loadLevel("jorge1.xml");
   GameplayManager = new CGameplayManager(*Level);
   DecorManager = new CDecorManager(*Level);
   CCameraControl *Camera = new CCameraControl();
   Camera->setPosition(SVector3(0, 0, 20));
   Camera->setLookDirection(SVector3(0, 0, -1));
   CApplication::get().getSceneManager().setActiveCamera(Camera);

   CApplication::get().getSceneManager().Lights.push_back(new CLight());
   CApplication::get().getSceneManager().Lights.back()->Color = SVector3(0.5f, 0.2f, 0.2f);
   CApplication::get().getSceneManager().Lights.back()->Position = SVector3(1.f, 2.f, 3.f);
}

void CGameState::OnRenderStart(float const Elapsed) {
   printf("Here in OnRenderStart\n");
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
