#include <stdio.h>

#include "CGameEventReceiver.h"

int numKilled;

CGameEventReceiver::CGameEventReceiver()
{
   numKilled = 0;
}

void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {

    //fprintf(stderr, "Removing enemy %d\n", Event.Enemy);
    //fprintf(stderr, "Removing renderable %d\n", Event.Renderable);

    SDeadEnemy DeadEnemy;
    DeadEnemy.DeathTimer = 0.5f;
    DeadEnemy.Renderable = Event.Enemy.Renderable;

    DeadEnemy.Renderable->setScale(SVector3(1.f, 1.0f, 0.4f));
    DeadEnemy.Renderable->setTranslation(DeadEnemy.Renderable->getTranslation() - SVector3(0.f, 0.5f, 0.f));
    DeadEnemies.push_back(DeadEnemy);
    
    //CApplication::get().getSceneManager().removeRenderable(Event.Renderable);
    
    numKilled++;
}

void CGameEventReceiver::OnGameTickStart(float const Elapsed)
{
    for (std::vector<SDeadEnemy>::iterator it = DeadEnemies.begin(); it != DeadEnemies.end();)
    {
        it->DeathTimer -= Elapsed;
        if (it->DeathTimer < 0.f)
        {
            CApplication::get().getSceneManager().removeRenderable(it->Renderable);
            it = DeadEnemies.erase(it);
        }
        else
            ++ it;
    }
}
