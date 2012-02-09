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



class CMainMenuState : public CState<CMainMenuState>
{
   public:

      freetype::font_data our_font;

      CTexture * woodTexture, * skyTexture;
      CMesh *logoMesh;
      CMeshRenderable *renderLogo;

      CMainMenuState();
      virtual void setupTextures();
      virtual void setupMeshes();

      virtual void drawSky(int backwards);

      virtual void drawButton();

      virtual void begin();
      virtual void end();
      virtual void OnRenderStart(float const Elapsed);
      virtual void OnRenderEnd(float const Elapsed);
      virtual void OnMouseEvent(SMouseEvent const & Event);
      virtual void OnKeyboardEvent(SKeyboardEvent const & Event);
};

#endif
