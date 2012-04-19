#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "CGameplayElement.h"
#include "CGameLevelLoader.h"

class CGameplayManager : public IUpdater {
   private:
      std::vector<CGameplayElement*> Elements;
      CCollisionEngine *PhysicsEngine;
   public:
      CGameplayManager(CGameLevel & level);
      void update(float);
};

#endif
