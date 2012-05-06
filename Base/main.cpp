//#include "CGameState.h"
#include "../StateMainMenu/CMainMenuState.h"
//#include "../StateOverworld/COverworldState.h"
//#include "../StateGame/CGameState.h"

#ifdef _WIN32
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "CabbageCore.lib")
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageParticles.lib")
#pragma comment(lib, "CabbageCollider.lib")
#pragma comment(lib, "CabbageGame.lib")
#pragma comment(lib, "CabbageGUI.lib")
#pragma comment(lib, "CabbageFramework.lib")
#pragma comment(lib, "StateMainMenu.lib")
#pragma comment(lib, "StateGame.lib")
#pragma comment(lib, "StateOverworld.lib")
#pragma comment(lib, "StateLWIB.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#endif

int main(int argc, char * argv[])
{
   CApplication & Application = CApplication::get();
   //Application.init(SPosition2(800, 600));
   Application.init(SPosition2(1200, 900));

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   CStateManager & StateManager = Application.getStateManager();
   //StateManager.setState(& CGameState::get());
   StateManager.setState(& CMainMenuState::get());

   Application.run();

   return 0;
}
