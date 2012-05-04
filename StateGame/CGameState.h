#ifndef __CGAMESTATE_HEADER_
#define __CGAMESTATE_HEADER_

#include <iostream>
#include "CGameplayManager.h"
#include "CDecorManager.h"
#include "CabbageFramework.h"
#include "CGameLevelLoader.h"

#include "../CabbageGame/CabbageGameInformation.h"
class CGameState : public CState<CGameState> {
   private:
      CGameplayManager *GameplayManager;
      CDecorManager *DecorManager;
      CGameLevel *Level;
   public:
      char const * LevelName;
      Cabbage::PlayerInformation Stats;
      CGameState();
      void begin();
      void OnRenderStart(float const Elapsed);
      void OnRenderEnd(float const Elapsed);
      void end();
      void OnKeyboardEvent(SKeyboardEvent const & Event);
      CGameLevel & getCurrentLevel();
};
#endif
