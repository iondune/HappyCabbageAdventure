#ifndef __CGAMESTATE_HEADER_
#define __CGAMESTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../StateMainMenu/CMainMenuState.h"
#include "../StateLWIB/CLWIBState.h"
#include "../StateLWIB/CBlock.h"
#include "../StateLWIB/CEnemy.h"
#include "../StateLWIB/CPlaceable.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "CGameplayManager.h"
#include "CGameEventReceiver.h"
#include "irrxml-1.2/src/irrXML.h"


#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CGameState : public CState<CGameState>
{
   CApplication & Application;
   CGameEventManager * GameEventManager;
   CGameEventReceiver GameEventReceiver;
   freetype::font_data our_font;

   public:
   CGameState();
   //Initalizer fxn
   ICamera *Camera;
   float fps, timeTotal;
   int numFrames, NumTreeTypes, NumFlowerTypes;
   int lowDef;
   const char * levelName;
   CGUIImageWidget *Health5, *Health4, *Health3, *Health2, *Health1;

   void EngineInit( void );
   void begin();
   void oldDisplay(); 
   void loadWorld(std::vector<CPlaceable*> *list);
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
   void GeneratePlants(float x, float y, float w, float h, float d);
   CMeshSceneObject* PrepEnemy(float x, float y);
};
#endif
