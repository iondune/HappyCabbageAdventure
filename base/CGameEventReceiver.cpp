#include "CGameEventReceiver.h"
int numKilled;
CGameEventReceiver::CGameEventReceiver() {
   numKilled = 0;
}
#include <stdio.h>
void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
   printf("Removing %d\n", Event.Renderable);
   CApplication::get().getSceneManager().removeRenderable((CMeshRenderable*)Event.Renderable);
   numKilled++;
}
