#ifndef ______CGAMELEVEL_OMG_HEADER
#define ______CGAMELEVEL_OMG_HEADER

class CGameplayElement;
class CElementBlockBreakable;
class CElementBlock;
class CElementEnemy;
class CElementPlayer;
class CElementItem;
class CElementBlockElevator;
class CElementBlockFlag;
class CGroundBlock;
class CBiggerBlock;

class CCollisionEngine;

#include "CabbageGameInformation.h"
#include "CGameLevelLoader.h"
#ifdef _ENABLED_CABBAGE_SOUND_
#include "sound.h"
#endif

namespace Env {
   enum Type {
      FOREST = 0,
      DESERT,
      WATER,
      ICE,
   };
}

class CGameLevel {
   friend CGameLevel & CGameLevelLoader::loadLevel(std::string, bool);

   public:
#ifdef _ENABLED_CABBAGE_SOUND_
      Mix_Chunk *dmgEnemy;
      Mix_Chunk *projectile;
#endif

   private:
      bool night, Loaded, Render;
      int env, numBlocks;
      int XmlCount;
#ifdef _ENABLED_CABBAGE_SOUND_
      Mix_Music *Soundtrack;
#endif

      std::vector<CGameplayElement*> ToDelete;
      std::vector<CGameplayElement*> Elements;
      std::vector<CElementBlock*> Blocks;
      std::vector<CElementBlockBreakable*> BreakableBlocks;
      std::vector<CGroundBlock*> GroundBlocks;
      std::vector<CBiggerBlock*> blocksFinal;
      std::vector<CElementEnemy*> Enemies;
      std::vector<CElementItem*> Items;
      std::vector<CElementBlockFlag*> Flags; 
      std::vector<CElementBlockElevator*> Elevators; 
      /*
      std::vector<CFriends*> Friends;
      */

      void toggleLoaded(); // Only available for CGameLevelLoader::loadLevel().
      void incrementXmlCount();
      CElementPlayer *PlayerElement;

      CCollisionEngine *PhysicsEngine;

   public:

      std::vector<CGameplayElement*> & getElements();


      std::vector<CGameplayElement*> & getGameplayElements();
      std::vector<CElementBlock*> & getBlocks();
      std::vector<CGroundBlock*> & getGroundBlocks();
      std::vector<CBiggerBlock*> & getConsolidatedBlocks();
      std::vector<CElementEnemy*> & getEnemies();
      std::vector<CElementItem*> & getItems();
      std::vector<CElementBlockFlag*> & getFlags();
      std::vector<CElementBlockElevator*> & getElevators();

      void setNoRender();
      bool shouldRender();
      /*
      std::vector<CPFriends*> & getFriends();
      std::vector<CElevator*> & getElevators();
      */

      CGameLevel();
      bool isLoaded();
      void setLoaded(bool);
      bool isNight();
      int getEnvironment();
      int getEnv();
      int  getXmlCount();
#ifdef _ENABLED_CABBAGE_SOUND_
      Mix_Music* getSoundtrack();
#endif

      CElementPlayer & getPlayer();
      void setPlayerInformation(Cabbage::PlayerInformation info);
      CCollisionEngine & getPhysicsEngine();
      void setPhysicsEngine(CCollisionEngine *p);

      void printInformation();

      void setupSoundtrack();

      void removeEnemy(CElementEnemy *Enemy);
      void addEnemy(CElementEnemy *Enemy);
      void removeItem(CElementItem* Item);
      void addItem(CElementItem* Item);
      void removeCollideable(CGameplayElement *Object);
      void addObject(CGameplayElement *Object);
};

#endif
