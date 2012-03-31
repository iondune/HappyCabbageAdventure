#ifndef __CMAIN____MENU_STATE_________H_____________
#define __CMAIN____MENU_STATE_________H_____________

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageFramework.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "../CabbageCore/CabbageCore.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageScene/CabbageScene.h"
#include "../StateLWIB/CLWIBState.h"
#include "../Base/CGameState.h"
#include "../StateOverworld/COverworldState.h"
#include "../CabbageSound/sound.h"
#include "../CabbageGUI/CabbageGUI.h"
#include "../Base/CGameplayManager.h"



class CMainMenuState : public CState<CMainMenuState>
{
   CApplication &Application;
   public:


      ICamera *Camera;

      CTexture * WoodTexture, * BackgroundTexture;
      CMesh *LogoMesh, *CubeMesh;
      CMeshSceneObject *RenderLogo, *RenderBackground;
      CGUIImageWidget *StartGame, *StartEditor, *ExitGame;
      CGUIFontWidget *StartFont, *EditorFont, *ExitFont;

      CMainMenuState();
      int mouse_x, mouse_y;
      char curDirection;
      float curAngle, maxX, maxY, sineValue;
      SPosition2 size;
      
      void setupTextures();
      void setupMeshes();
      void setupButtons();

      void drawButton();
      void rotateButton();

      void begin();
      void end();
      void OnRenderStart(float const Elapsed);
      void OnRenderEnd(float const Elapsed);
      void OnKeyboardEvent(SKeyboardEvent const & Event);
      void OnWidgetClick(CGUIWidget * Widget);
      void OnWidgetHover(CGUIWidget * Widget);
      void OnWidgetUnHover(CGUIWidget * Widget);
};

#endif
