#ifndef __CGAMELEVELEVENTRECEIVER_H_
#define __CGAMELEVELEVENTRECEIVER_H_

#include "sigslot.h"

class CGameLevelEventReceiver : public sigslot::has_slots<>
{
    CGameLevelEventReceiver();
    public:
    void RemoveEnemy(SRemoveEnemyEvent const & Event);
    void RemoveItem(SRemoveItemEvent const & Event);
    void AddEnemy(SAddEnemyEvent const & Event);
    void AddItem(SAddItemEvent const & Event);
};

#endif
