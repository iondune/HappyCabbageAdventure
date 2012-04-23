#ifndef _CGAMELEVELEVENTMANAGER_HEADER_
#define _CGAMELEVELEVENTMANAGER_HEADER_

struct SRemoveEnemyEvent {
   CElementEnemy *Enemy;
}

struct SRemoveItemEvent {
   CElementItem *Item;
}

struct SAddEnemyEvent {
   CElementEnemy *Enemy;
}

struct SAddItemEvent {
   CElementItem *Item;
}

class CGameLevelEventManager
{

public:

    sigslot::signal1<SRemoveEnemyEvent const &> RemoveEnemy;
    sigslot::signal1<SRemoveItemEvent const &> RemoveItem;
    sigslot::signal1<SAddEnemyEvent const &> AddEnemy;
    sigslot::signal1<SAddItemEvent const &> AddItem;
};


#endif
