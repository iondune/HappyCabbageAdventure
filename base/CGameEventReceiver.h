#ifndef __CGAMEEVENTRECEIVER_H_
#define __CGAMEEVENTRECEIVER_H_

#include "CGameEventManager.h"
#include "../CabbageFramework/CApplication.h"

extern int numKilled;

class CGameState;
class CGameEventReceiver : public sigslot::has_slots<> {
   friend class CGameState;
   CGameEventReceiver();

   public:
      void OnEnemyDeath(SEnemyDeathEvent const & Event);
};

#endif
