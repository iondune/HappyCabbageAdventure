#ifndef ______CGAMELEVEL_OMG_HEADER
#define ______CGAMELEVEL_OMG_HEADER

class CGameplayElement;
class CElementBlock;
class CElementEnemy;

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"
#include "CGameLevelLoader.h"

class CGameLevel {
   friend CGameLevel & CGameLevelLoader::loadLevel(std::string, bool);
   private:
      bool night, Loaded;
      int env, numBlocks;

      std::vector<CGameplayElement*> Elements;
      std::vector<CElementBlock*> Blocks;
      std::vector<CGroundBlock*> GroundBlocks;
      std::vector<CBiggerBlock*> blocksFinal;
      std::vector<CElementEnemy*> Enemies;
      /*
      std::vector<CItem*> Items;
      std::vector<CFriends*> Friends;
      std::vector<CElevator*> Elevators;
      */

      void toggleLoaded(); // Only available for CGameLevelLoader::loadLevel().

   public:
      std::vector<CGameplayElement*> & getElements();
      std::vector<CElementBlock*> & getBlocks();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();
      std::vector<CElementEnemy*> & getEnemies();

      /*
      std::vector<CPFriends*> & getFriends();
      std::vector<CPItem*> & getItems();
      std::vector<CElevator*> & getElevators();
      */

      CGameLevel();
      bool isLoaded();
      bool isNight();
      int getEnvironment();
      int getEnv();
};

#endif
