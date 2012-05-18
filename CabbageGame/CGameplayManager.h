#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "IUpdater.h"
#include <vector>

class CCollisionEngine;

class CGameLevel;
class CElementPlayer;
class CGameplayElement;
class CGameplayGUIManager;

class CGameplayManager : public IUpdater {
   private:
      CGameLevel &Level;
      std::vector<CGameplayElement*> & Elements;
      CGameplayGUIManager *GUIManager;
      CCollisionEngine *PhysicsEngine;
      CElementPlayer *Player;
   public:
      CGameplayManager(CGameLevel & level);
      void update(float);
      void OnEnd();
};

#endif
