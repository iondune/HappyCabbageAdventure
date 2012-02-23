#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>
#include <vector>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameState.h"
//#include "../MainMenu/CMainMenuState.h"

#define NUM_LEVELS 6
#define NUM_CAMERAS 2
#define TRANSITION_PERIOD 0.65f

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
   CMeshSceneObject *renderMap, *discRender, *playerRender;

   int startx, starty;
   float pitchphi, yawtheta;
   int mouseDown;


   SVector3 eye, look, eyeShift, lookShift, eyeTarget, lookTarget;
   int WindowWidth, WindowHeight;
   SVector3 rot;

   float transitionTimer;

   SomeLevel levels [NUM_LEVELS];
   SVector3 cameraPos [NUM_CAMERAS];

   void LoadShaders();
   void PrepMeshes();
   void loadLevels();
   void setCameraTrans();
   void levelIcons(SVector3 loc, CMesh *levelIcon, int iconColor);

   CLight * PlayerLight;

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
   void superInterpolator(SVector3 & curr, SVector3 & change, SVector3 & target, float delta);
   void shiftSetter(SVector3 & curr, SVector3 & change, SVector3 & target);
   
};
#endif
