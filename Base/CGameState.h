#ifndef __CGAMESTATE_HEADER_
#define __CGAMESTATE_HEADER_

#include <iostream>
#include "CGameplayManager.h"
#include "CDecorManager.h"
#include "CabbageFramework.h"

class CGameState : public CState<CGameState> {
   private:
      CGameplayManager *GameplayManager;
      CDecorManager *DecorManager;
   public:
      CGameState();
      void begin();
      void OnRenderStart(float const Elapsed);
      void OnRenderEnd(float const Elapsed);
      void end();
};
#endif
