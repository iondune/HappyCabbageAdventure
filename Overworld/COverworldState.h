#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../MainMenu/CMainMenuState.h"
#include "../CLWIB/CLWIBState.h"
#include "CGameEventReceiver.h"

class COverworldState : public CState<COverworldState>
{
   CApplication & Application;
//   CGameEventManager * GameEventManager;
//   CGameEventReceiver GameEventReceiver;
   freetype::font_data our_font;

   public:
   COverworldState();
   //Initalizer fxn
   void begin();
   CCamera *Camera;
   float fps, timeTotal;
   int numFrames;
   //Runs at very start of display
   void OnRenderStart(float const Elapsed);
   //Sends event every time key pressed (also when held)
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   //Runs at program close (currently not implemented)
   void end();
};
#endif
