#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "CGameplayElement.h"
#include "CGameLevelLoader.h"

class CGameLevel;
class CElementPlayer;
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
};

#endif
