#ifndef __CGAMELEVELLOADER_HEADER
#define __CGAMELEVELLOADER_HEADER

#include "irrxml-1.2/src/irrXML.h"

class CBlock;
class CEnemy;
class CPItem;
class CPFriends;
class CPlaceable;
class CGameLevel;

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"

class CGameLevelLoader {
   private:
      static std::map<std::string, CGameLevel*> LoadedLevels;
      static std::vector<CBiggerBlock*> consolidateBlocks(std::vector<CBiggerBlock*> blocksY);

   public:
      static CGameLevel &loadLevel(const char*);
      static CGameLevel &loadLevel(const char*, bool);
};

#endif
