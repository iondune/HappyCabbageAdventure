#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "CGameplayElement.h"
#include "CGameLevelLoader.h"

class CGameLevel;
class CElementPlayer;

class CGameplayManager : public IUpdater {
   private:
      std::vector<CGameplayElement*> Elements;
      CCollisionEngine *PhysicsEngine;
      CElementPlayer *Player;
   public:
      CGameplayManager(CGameLevel & level);
      void update(float);
};

#endif
