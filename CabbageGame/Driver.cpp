#include "CGameLevelLoader.h"
#include "CGameLevel.h"
#include "CGameplayManager.h"

int main(int argc, char* argv[]) {
   CGameLevel *Level;
   if(argc > 1) 
      Level = &CGameLevelLoader::loadLevel(argv[1]);
   else
      Level = &CGameLevelLoader::loadLevel("jorge1.xml");
   CGameplayManager *GameplayManager = new CGameplayManager(*Level);
   Level->printInformation();

   return 0;
}
