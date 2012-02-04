#include "CGameEventReceiver.h"
int numKilled;
CGameEventReceiver::CGameEventReceiver() {
   numKilled = 0;
}
void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
   CApplication::get().getSceneManager().removeRenderable((CMeshRenderable*)Event.Renderable);
   numKilled++;
}
