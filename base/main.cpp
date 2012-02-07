#include "CGameState.h"
#include "../MainMenu/CMainMenuState.h"

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
    Application.init(SPosition2(1024, 768));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState(& CGameState::get());

    Application.run();

    return 0;
}

