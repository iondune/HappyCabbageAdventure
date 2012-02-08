#ifndef __CGAMESTATE_HEADER_
#define __CGAMESTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../MainMenu/CMainMenuState.h"


#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CGameState : public CState<CGameState>
{
   CApplication & Application;
   CGameEventManager * GameEventManager;
   CGameEventReceiver GameEventReceiver;

   public:
   CGameState();
   void EngineInit( void );
   //Initalizer fxn
   void begin();
   CCamera *Camera;
   float fps, timeTotal;
   int numFrames;
   void oldDisplay(); 
   //Runs at very start of display
   void OnRenderStart(float const Elapsed);
   //Sends event every time key pressed (also when held)
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   //Runs at program close (currently not implemented)
   void end();
   void PrepShadow();
   void PrepBlock(float x, float y, float w, float h);
   void PrepGrass(float x, float y, float w, float h);
   void PrepSky();
   CMeshRenderable* PrepEnemy(float x, float y);
};
#endif
