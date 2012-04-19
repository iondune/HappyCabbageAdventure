#ifndef ______CGAMELEVEL_OMG_HEADER
#define ______CGAMELEVEL_OMG_HEADER

class CGameplayElement;
class CElementBlock;

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"
#include "CGameLevelLoader.h"

class CGameLevel {
   friend CGameLevel & CGameLevelLoader::loadLevel(const char*, bool);
   private:
      bool night;
      int env, numBlocks;

      std::vector<CGameplayElement*> Elements;
      std::vector<CElementBlock*> Blocks;
      std::vector<CGroundBlock*> GroundBlocks;
      std::vector<CBiggerBlock*> blocksFinal;
      /*
      std::vector<CFriends*> Friends;
      std::vector<CItem*> Items;
      std::vector<CEnemy*> Enemies;
      std::vector<CElevator*> Elevators;
      */

   public:
      std::vector<CGameplayElement*> & getElements();
      std::vector<CElementBlock*> & getBlocks();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();
      /*
      std::vector<CPFriends*> & getFriends();
      std::vector<CPItem*> & getItems();
      std::vector<CEnemy*> & getEnemies();
      std::vector<CElevator*> & getElevators();
      */

      bool isNight();
      int getEnvironment();
      int getEnv();
};

#endif
