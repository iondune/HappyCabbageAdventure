#include "CGameLevelLoader.h"
#include "CGameLevel.h"
#include "CGameplayManager.h"

int main(int argc, char* argv[]) {
   printf("Here\n");
   CGameLevel *Level = &CGameLevelLoader::loadLevel("jorge1.xml");
   printf("Here2\n");
   CGameplayManager *GameplayManager = new CGameplayManager(*Level);
   printf("Here3\n");

   return 0;
}
