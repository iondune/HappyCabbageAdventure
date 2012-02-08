//#include "CGameState.h"
#include "../MainMenu/CMainMenuState.h"






#ifdef _WIN32
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageCollider.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
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
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(800, 600));
    //Application.init(SPosition2(1024, 768));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState(& CMainMenuState::get());

    Application.run();

    return 0;
}

