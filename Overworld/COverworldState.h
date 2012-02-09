#ifndef __COVERWORLDSTATE_HEADER_
#define __COVERWORLDSTATE_HEADER_

#include <iostream>

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
/*
#include "../base/CGameState.h"
#include "../MainMenu/CMainMenuState.h"
*/

class COverworldState : public CState<COverworldState>
{
   CApplication & Application;
//   CGameEventManager * GameEventManager;
//   CGameEventReceiver GameEventReceiver;

//Boolean integers for keypressing
   int aDown, dDown, spaceDown, wDown, sDown;

   CShader *Flat, *Diffuse, *DiffuseTexture;
   CMeshRenderable *renderMap;
   CMesh *mapMesh;



   int WindowWidth, WindowHeight;

   //std::vector<Node> nodes;

   void LoadShaders();
   void PrepMeshes();

   SLight * PlayerLight;

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
