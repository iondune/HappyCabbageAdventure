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

      std::vector<CPlaceable*> Placeables;
      std::vector<CPFriends*> Friends;
      std::vector<CPItem*> Items;
      std::vector<CBlock*> Blocks;
      std::vector<CEnemy*> Enemies;
      std::vector<CGroundBlock*> GroundBlocks;

   public:
      std::vector<CPlaceable*> & getPlaceables();
      std::vector<CPFriends*> & getFriends();
      std::vector<CPItem*> & getItems();
      std::vector<CBlock*> & getBlocks();
      std::vector<CEnemy*> & getEnemies();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();

      bool isNight();
      int getEnvironment();
      int getEnv();
};

#endif
