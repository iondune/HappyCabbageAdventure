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

#include "../base/CGameState.h"

class CMainMenuState : public CState<CMainMenuState>
{
   public:

      CMainMenuState();
      CTexture * woodTexture, * skyTexture;

      virtual void setupTextures();

      virtual void drawSky(int backwards);

      virtual void drawButton();

      virtual void begin();
      virtual void end();
      virtual void OnRenderStart(float const Elapsed);
      virtual void OnRenderEnd(float const Elapsed);
      virtual void OnMouseEvent(SMouseEvent const & Event);
      virtual void OnKeyboardEvent(SKeyboardEvent const & Event);
};
