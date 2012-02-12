#ifndef __CMAIN____MENU_STATE_________H_____________
#define __CMAIN____MENU_STATE_________H_____________

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "../CabbageCore/CabbageCore.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CLWIB/CLWIBState.h"
#include "../base/CGameState.h"
#include "../Overworld/COverworldState.h"



class CMainMenuState : public CState<CMainMenuState>
{
   CApplication &Application;
   public:

      freetype::font_data our_font;

      CCamera *Camera;

      CTexture * woodTexture, * skyTexture;
      CMesh *logoMesh;
      CMeshRenderable *renderLogo;

      CMainMenuState();
      int mouse_x, mouse_y;
      char curDirection;
      float curAngle;
      
      void setupTextures();
      void setupMeshes();

      void drawSky(int backwards);

      void drawButton();
      void rotateButton();

      void begin();
      void end();
      void OnRenderStart(float const Elapsed);
      void OnRenderEnd(float const Elapsed);
      void OnMouseEvent(SMouseEvent const & Event);
      void OnKeyboardEvent(SKeyboardEvent const & Event);
};

#endif
