#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "CGameplayElement.h"

class CGameplayManager : public IUpdater {
   private:
      std::vector<CGameplayElement*> Elements;
      CEngine *PhysicsEngine;
   public:
      CGameplayManager();
      void update(float);
};

#endif
