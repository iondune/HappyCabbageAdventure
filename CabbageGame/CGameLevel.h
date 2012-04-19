#ifndef ______CGAMELEVEL_OMG_HEADER
#define ______CGAMELEVEL_OMG_HEADER

class CBlock;
class CEnemy;
class CPItem;
class CPFriends;
class CPlaceable;

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"
#include "CGameLevelLoader.h"

class CGameLevel {
   friend CGameLevel & CGameLevelLoader::loadLevel(const char*, bool);
   private:
      bool night;
      int env, numBlocks;
      std::vector<CBiggerBlock*> blocksFinal;

      std::vector<CGameplayElement*> Elements;
      std::vector<CFriends*> Friends;
      std::vector<CItem*> Items;
      std::vector<CBlock*> Blocks;
      std::vector<CEnemy*> Enemies;
      std::vector<CGroundBlock*> GroundBlocks;
      std::vector<CElevator*> GroundBlocks;

   public:
      std::vector<CGameplayElement*> & getElements();
      std::vector<CPFriends*> & getFriends();
      std::vector<CPItem*> & getItems();
      std::vector<CBlock*> & getBlocks();
      std::vector<CEnemy*> & getEnemies();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CElevator*> & getElevators();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();

      bool isNight();
      int getEnvironment();
      int getEnv();
};

#endif
