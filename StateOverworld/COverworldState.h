#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>
#include <vector>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageGame/CabbageGameInformation.h"

#define NUM_LEVELS 7
#define NUM_CAMERAS 3
#define TRANSITION_PERIOD 0.45f
#define RAD_TO_DEG 180.0f/3.145f
#define DEG_TO_RAD 3.145f/180.0f

class COverworldState : public CState<COverworldState>
{
  struct SomeLevel
  {
    SVector3f loc;
    char const * name;
    bool completed;
  };

   CApplication & Application;
//   CGameEventManager * GameEventManager;
//   CGameEventReceiver GameEventReceiver;

   //Boolean integers for keypressing
   int aDown, dDown, spaceDown, wDown, sDown, yDown;
   float changex, changey, changez, sineValue;
   SVector3f playerVector, playerVecShift, playerVecTarget;

   CShader *Flat, *Diffuse, *ToonTexture;
   CShader *DeferredFlat, * DeferredDiffuse, * DeferredTexture;
   CMeshSceneObject *renderMap, *discRender, *playerRender,
                    *arrowRender1, *arrowRender2, *SkySphere;

   int startx, starty;
   float pitchphi, yawtheta;
   int mouseDown;


   bool isDay, transitionWorld;
   SVector3f eye, look, eyeShift, lookShift, eyeTarget, lookTarget;
   int WindowWidth, WindowHeight;
   SVector3f rot;

   float transitionTimer;

   SomeLevel levels [NUM_LEVELS];
   SVector3f cameraPos [NUM_CAMERAS];
   bool levelsUnlocked;

   void LoadShaders();
   void addMeshes(SVector3f loc, CMesh *newMesh, CTexture *texture);
   void PrepMeshes();
   void PrepSkySphere();
   void loadLevels();
   void setCameraTrans();
   void levelIcons(SVector3f loc, CMesh *levelIcon, int iconColor);
   void testFun();

   CLight * PlayerLight;

   public:

   Cabbage::PlayerInformation Stats, NewStats;
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
   void superInterpolator(SVector3f & curr, SVector3f & change, float delta, float transLength);
   void shiftSetter(SVector3f & curr, SVector3f & change, SVector3f & target);
   float angleMaker(SVector3f start, SVector3f toPoint);
};
#endif
