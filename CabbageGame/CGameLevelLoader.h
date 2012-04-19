#ifndef __CGAMELEVELLOADER_HEADER
#define __CGAMELEVELLOADER_HEADER

#include "irrxml-1.2/src/irrXML.h"

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"

class CGameLevelLoader {
   private:
      static std::map<std::string, CGameLevel*> LoadedLevels;
      static std::vector<CBiggerBlock*> consolidateBlocks(std::vector<CBiggerBlock*> blocksY);
      static CGameLevel *LatestLevel;

   public:
      static CGameLevel &loadLevel(const char*);
      static CGameLevel &loadLevel(const char*, bool);
      static CGameLevel &getLatestLevel();
};

#endif
