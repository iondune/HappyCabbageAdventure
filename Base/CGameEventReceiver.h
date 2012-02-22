#ifndef __CGAMEEVENTRECEIVER_H_
#define __CGAMEEVENTRECEIVER_H_

#include "CGameEventManager.h"
#include "../CabbageFramework/CApplication.h"

extern int numKilled;

class CGameEventReceiver : public sigslot::has_slots<>
{

    friend class CGameState;

    CGameEventReceiver();

    struct SDeadEnemy
    {
        ISceneObject * Renderable;
        float DeathTimer;
    };

    std::vector<SDeadEnemy> DeadEnemies;

    public:

        void OnEnemyDeath(SEnemyDeathEvent const & Event);

        void OnGameTickStart(float const Elapsed);

};

#endif
