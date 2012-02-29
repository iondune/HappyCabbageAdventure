#include <stdio.h>

#include "CGameEventReceiver.h"

int numKilled;

CGameEventReceiver::CGameEventReceiver()
{
   numKilled = 0;
   printf("Here!!!\n");
   DeadEnemies.clear();
}

void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
    //fprintf(stderr, "Removing enemy %d\n", Event.Enemy);
    //fprintf(stderr, "Removing renderable %d\n", Event.Renderable);

    SDeadEnemy DeadEnemy;
    DeadEnemy.DeathTimer = 6.f;
    DeadEnemy.Renderable = Event.Enemy.Renderable;

    DeadEnemy.Renderable->setScale(SVector3(1.f, 1.0f, 0.4f));
    DeadEnemy.Renderable->setTranslation(DeadEnemy.Renderable->getTranslation() - SVector3(0.f, 0.5f, 0.f));

    DeadEnemy.ParticleE = new CParticleEngine(DeadEnemy.Renderable->getTranslation(), 100, 6, DEATH_PARTICLE);

    DeadEnemies.push_back(DeadEnemy);
    
    //CApplication::get().getSceneManager().removeSceneObject(Event.Renderable);
    
    numKilled++;
}

void CGameEventReceiver::OnGameTickStart(float const Elapsed)
{
    int i = 0;
    for (std::vector<SDeadEnemy>::iterator it = DeadEnemies.begin(); it != DeadEnemies.end();)
    {
        it->DeathTimer -= Elapsed;
        if(it->ParticleE)
           it->ParticleE->step(Elapsed);
        if (it->DeathTimer < 0.f)
        {
            CApplication::get().getSceneManager().removeSceneObject(it->Renderable);
            it->ParticleE->deconstruct();
            delete it->ParticleE;
            it->ParticleE = NULL;
            it = DeadEnemies.erase(it);
        }
        else
            ++ it;
    }
}
