#ifndef __CGAMELEVELLOADER_HEADER
#define __CGAMELEVELLOADER_HEADER

#include "irrxml-1.2/src/irrXML.h"

#include "CBlock.h"
#include "CEnemy.h"
#include "CPItem.h"
#include "CPFriends.h"
#include "CPlaceable.h"

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"

class CGameLevelLoader {
   private:
      bool loaded, night;
      int env, numBlocks;
      std::vector<CBiggerBlock*> blocksY, blocksFinal;
      std::vector<CPlaceable*> Placeables;
      std::vector<CPFriends*> Friends;
      std::vector<CPItem*> Items;
      std::vector<CBlock*> Blocks;
      std::vector<CEnemy*> Enemies;
      std::vector<CGroundBlock*> GroundBlocks;

      void consolidateAndAddBlocks();

   public:
      std::vector<CPlaceable*> & getPlaceables();
      std::vector<CPFriends*> & getFriends();
      std::vector<CPItem*> & getItems();
      std::vector<CBlock*> & getBlocks();
      std::vector<CEnemy*> & getEnemies();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();

      CGameLevelLoader(const char*);

      bool isNight();
      int getEnv();
};

#endif
