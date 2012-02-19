#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>
#include <vector>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameState.h"
//#include "../MainMenu/CMainMenuState.h"

#define NUM_LEVELS 2

class COverworldState : public CState<COverworldState>
{
  struct SomeLevel
  {
    SVector3 loc;
    const char * name;
  };

   CApplication & Application;
//   CGameEventManager * GameEventManager;
//   CGameEventReceiver GameEventReceiver;

   //Boolean integers for keypressing
   int aDown, dDown, spaceDown, wDown, sDown;
   float changex, changey, changez;
   SVector3 playerVector;

   CShader *Flat, *Diffuse, *DiffuseTexture;
   CMeshRenderable *renderMap, *discRender, *playerRender;

   int startx, starty;
   float pitchphi, yawtheta;
   int mouseDown;


   SVector3 eye, look;
   int WindowWidth, WindowHeight;
   SVector3 rot;

   SomeLevel levels [NUM_LEVELS];

   void LoadShaders();
   void PrepMeshes();
   void loadLevels();
   void levelIcons(SVector3 loc, CMesh *levelIcon, int iconColor);

   SLight * PlayerLight;

   public:

   CCamera *Camera;
   float camRotValue, fps, timeTotal;
   int numFrames, curNode;

   COverworldState();
   //Initalizer fxn
   void begin();
   void step(float delta);
   //Runs at very start of display
   void OnRenderStart(float const Elapsed);
   //Sends event every time key pressed (also when held)
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   //Runs at program close (currently not implemented)
   void end();
   void bouncePlayer();
   void movePlayer();
   /*
   void stepCamera(float delta);
   void OnMouseEvent(SMouseEvent const & Event);
   */
};
#endif
