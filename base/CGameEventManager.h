#ifndef _CABBAGE_BASE_CGAMEEVENTMANAGER_H_
#define _CABBAGE_BASE_CGAMEEVENTMANAGER_H_


#include "../CabbageFramework/sigslot/sigslot.h"

#include "CGameplayManager.h"

struct SEnemyDeathEvent
{
    CGameplayManager::SEnemy * Enemy;
};

struct SEnemyDamagedEvent
{
    CGameplayManager::SEnemy * Enemy;
};

struct SPlayerDeathEvent
{
    CGameplayManager::SEnemy * KilledBy;
    // We can have other members here if its possible 
    // for the player to die from something else
};

struct SPlayerDamagedEvent
{
    CGameplayManager::SEnemy * DamagedBy;
};

class CGameEventManager
{

public:

    sigslot::signal1<SEnemyDeathEvent const &> OnEnemyDeath;

    sigslot::signal1<SEnemyDamagedEvent const &> OnEnemyDamaged;

    sigslot::signal1<SPlayerDeathEvent const &> OnPlayerDeath;

    sigslot::signal1<SPlayerDamagedEvent const &> OnPlayerDamaged;

};

typedef sigslot::has_slots<> CGameEventReceiver;

#endif
