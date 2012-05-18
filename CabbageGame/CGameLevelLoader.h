#ifndef __CGAMELEVELLOADER_HEADER
#define __CGAMELEVELLOADER_HEADER

#include "irrxml-1.2/src/irrXML.h"
#include <map>
#include <string>
#include <vector>

class CBiggerBlock;
class CGameLevel;

class CGameLevelLoader {
   private:
      static std::map<std::string, CGameLevel*> LoadedLevels;
      static std::string LevelDirectory;
      static std::vector<CBiggerBlock*> consolidateBlocks(std::vector<CBiggerBlock*> blocksY);
      static CGameLevel *LatestLevel;

   public:
      static CGameLevel &loadLevel(std::string);
      static CGameLevel &loadLevel(std::string, bool);
      static CGameLevel &getLatestLevel();
      static std::string getLevelDirectory();
      static void resetLevel();
};

#endif
