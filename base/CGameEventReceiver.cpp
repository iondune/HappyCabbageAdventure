#include <stdio.h>

#include "CGameEventReceiver.h"

int numKilled;

CGameEventReceiver::CGameEventReceiver()
{
   numKilled = 0;
}

void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
    fprintf(stderr, "Removing enemy %d\n", Event.Enemy);
    fprintf(stderr, "Removing renderable %d\n", Event.Renderable);

    //CApplication::get().getSceneManager().removeRenderable(Event.Renderable);
    
    numKilled++;
}
