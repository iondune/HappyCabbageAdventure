#ifndef __CGAMEEVENTRECEIVER_H_
#define __CGAMEEVENTRECEIVER_H_

#include "CGameEventManager.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "../CabbageFramework/CApplication.h"

extern int numKilled;

class CGameEventReceiver : public sigslot::has_slots<>
{

    friend class CGameState;

    CGameEventReceiver();

    struct SDeadEnemy
    {
        ISceneObject * Renderable;
        CParticleEngine *ParticleE;

        float DeathTimer;
    };

    std::vector<SDeadEnemy> DeadEnemies;
    CParticleEngine *playerDamagedEngine;

    public:

        void OnEnemyDeath(SEnemyDeathEvent const & Event);
        void OnPlayerDamaged(SPlayerDamagedEvent const & Event);
        void OnGameTickStart(float const Elapsed);

};

#endif
