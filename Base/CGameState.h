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
#include "../CabbageGUI/CabbageGUI.h"
#include "CGameplayManager.h"
#include "CGameEventReceiver.h"
#include "irrxml-1.2/src/irrXML.h"
#include "CBiggerBlock.h"


#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CGameState : public CState<CGameState>
{
   CApplication & Application;
   CGameEventManager * GameEventManager;
   CGameEventReceiver GameEventReceiver;

   CGUIFontWidget *GameOverText, *GameOverText2, * GameWinText, *RestartGameText, *RestartGameText2, *LivesText, *LivesText2, *fpsText, *fpsText2;

   public:
   CGameState();
   //Initalizer fxn
   ICamera *Camera;
   float fps, timeTotal, prevEnergy, energyStatus, moveDown, StartWin, curScaleX, curScaleY;
   int numFrames, NumTreeTypes, NumFlowerTypes, lowDef, prevHealth, numBlocks;
   const char * levelName;
   bool oldFern, launch;
   CGUIImageWidget *Health5, *Health4, *Health3, *Health2, *Health1, *CabbageFace, *CabbageEnergyBar, *CabbageMeter, *CabbageHurtFace;
   std::vector<CBiggerBlock*> blocksY, blocksX, blocksFinal;

   CParticleEngine *f1, *f2, *f3, *glow;

   void consolidateAndAddBlocks();
   void EngineInit( void );
   void begin();
   void LoadHUD();
   void oldDisplay();
   void UpdateLeaves();
   void UpdateEnergy(float const Elapsed);
   void loadWorld(std::vector<CPlaceable*> *list);
   //Runs at very start of display
   void OnRenderStart(float const Elapsed);
   //Sends event every time key pressed (also when held)
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   void Initialize();
   //Runs at program close (currently not implemented)
   void end();
   void PrepShadow();
   void PrepBlock(float x, float y, float w, float h);
   void PrepGrass(float x, float y, float w, float h);
   void PrepSky();
   void GeneratePlants(float x, float y, float w, float h, float d);
   CMeshSceneObject* PrepEnemy(float x, float y);
   void RunVictorySequence(float Elapsed);

   void setLodLevel(int const level);
};
#endif
