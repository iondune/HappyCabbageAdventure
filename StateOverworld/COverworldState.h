#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>
#include <vector>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameState.h"
//#include "../MainMenu/CMainMenuState.h"

#define NUM_LEVELS 7
#define NUM_CAMERAS 3
#define TRANSITION_PERIOD 0.45f
#define RAD_TO_DEG 180.0f/3.145f
#define DEG_TO_RAD 3.145f/180.0f

class COverworldState : public CState<COverworldState>
{
  struct SomeLevel
  {
    SVector3 loc;
    const char * name;
    bool completed;
  };

   CApplication & Application;
//   CGameEventManager * GameEventManager;
//   CGameEventReceiver GameEventReceiver;

   //Boolean integers for keypressing
   int aDown, dDown, spaceDown, wDown, sDown, yDown;
   float changex, changey, changez, sineValue;
   SVector3 playerVector, playerVecShift, playerVecTarget;

   CShader *Flat, *Diffuse, *ToonTexture;
   CShader *DeferredFlat, * DeferredDiffuse, * DeferredTexture;
   CMeshSceneObject *renderMap, *discRender, *playerRender,
                    *arrowRender1, *arrowRender2, *SkySphere;

   int startx, starty;
   float pitchphi, yawtheta;
   int mouseDown;


   bool isDay, transitionWorld;
   SVector3 eye, look, eyeShift, lookShift, eyeTarget, lookTarget;
   int WindowWidth, WindowHeight;
   SVector3 rot;

   float transitionTimer;

   SomeLevel levels [NUM_LEVELS];
   SVector3 cameraPos [NUM_CAMERAS];
   bool levelsUnlocked;

   void LoadShaders();
   void addMeshes(SVector3 loc, CMesh *newMesh, CTexture *texture);
   void PrepMeshes();
   void PrepSkySphere();
   void loadLevels();
   void setCameraTrans();
   void levelIcons(SVector3 loc, CMesh *levelIcon, int iconColor);
   void testFun();

   CLight * PlayerLight;

   public:

   bool newGame,levelCompleted;

   ICamera *Camera;
   float camRotValue, fps, timeTotal, stepValue;
   int numFrames, curNode, curCamera;

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
   void superInterpolator(SVector3 & curr, SVector3 & change, float delta, float transLength);
   void shiftSetter(SVector3 & curr, SVector3 & change, SVector3 & target);
   float angleMaker(SVector3 start, SVector3 toPoint);
   
};
#endif
