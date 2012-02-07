#include "CGameState.h"
#include "../MainMenu/CMainMenuState.h"

int main(int argc, char *argv[])
{
   CApplication & Application = Application.get();
   Application.init(SPosition2(1024, 768));

   Application.getStateManager().setState<CGameState>();

   Application.run();

   return 0;
}

