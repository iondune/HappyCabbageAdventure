//#include "CGameState.h"
#include "../StateMainMenu/CMainMenuState.h"
#include "../StateOverworld/COverworldState.h"

#ifdef _WIN32
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageCollider.lib")
#pragma comment(lib, "CabbageFramework.lib")
#pragma comment(lib, "StateMainMenu.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#endif
// Old
/*
int main(int argc, char *argv[])
{
   CApplication & Application = Application.get();
   Application.init(SPosition2(1024, 768));

   Application.getStateManager().setState<CGameState>();

   Application.run();

   return 0;
}
*/

int main(int argc, char * argv[])
{
   printf("Argc: %d %s\n", argc, argv[1]);
   if(argc > 1 && strcmp(argv[1], "-l") == 0) {
      CGameState::get().lowDef = 1;
      printf("Low Definition Mode: On\n");
   }
   else {
      CGameState::get().lowDef = 0;
      printf("Low Definition Mode: Off\n");
   }

    CApplication & Application = CApplication::get();
    //Application.init(SPosition2(800, 600));
    Application.init(SPosition2(1200, 900));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState(& CMainMenuState::get());
    //StateManager.setState(& COverworldState::get());

    Application.run();

    return 0;
}

