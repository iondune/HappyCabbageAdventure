#ifndef ______CGAMELEVEL_OMG_HEADER
#define ______CGAMELEVEL_OMG_HEADER

class CGameplayElement;
class CElementBlock;
class CElementEnemy;
class CElementPlayer;
class CElementItem;
class CElementBlockFlag;

#include "CDecorManager.h" //For CGroundBlock
#include "CBiggerBlock.h"
#include "CGameLevelLoader.h"

class CGameLevel {
   friend CGameLevel & CGameLevelLoader::loadLevel(std::string, bool);
   private:
      bool night, Loaded, Render;
      int env, numBlocks;
      int XmlCount;

      std::vector<CGameplayElement*> Elements;
      std::vector<CElementBlock*> Blocks;
      std::vector<CGroundBlock*> GroundBlocks;
      std::vector<CBiggerBlock*> blocksFinal;
      std::vector<CElementEnemy*> Enemies;
      std::vector<CElementItem*> Items;
      std::vector<CElementBlockFlag*> Flags; 
      /*
      std::vector<CFriends*> Friends;
      std::vector<CElevator*> Elevators;
      */

      void toggleLoaded(); // Only available for CGameLevelLoader::loadLevel().
      void incrementXmlCount();
      CElementPlayer *PlayerElement;

      CCollisionEngine *PhysicsEngine;

   public:
      std::vector<CGameplayElement*> & getElements();
      std::vector<CElementBlock*> & getBlocks();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();
      std::vector<CElementEnemy*> & getEnemies();
      std::vector<CElementItem*> & getItems();
      std::vector<CElementBlockFlag*> & getFlags();

      void setNoRender();
      bool shouldRender();
      /*
      std::vector<CPFriends*> & getFriends();
      std::vector<CElevator*> & getElevators();
      */

      CGameLevel();
      bool isLoaded();
      bool isNight();
      int getEnvironment();
      int getEnv();
      int  getXmlCount();

      CElementPlayer & getPlayer();
      CCollisionEngine & getPhysicsEngine();

      void printInformation();
};

#endif
