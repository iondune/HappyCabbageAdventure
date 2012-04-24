#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CCollisionEngine.h"
//#include "../CabbageSound/sound.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "CPlayerView.h"

#include "CItem.h"
#include "CIHealth.h"
#include "EApple.h"
#include "CFriends.h"

class CGameEventManager;
class CRenderable;
class EApple;
class CItem;
class CIHealth;
class CFriends;

class EKiwi;

class CGameplayManager : public ICollisionResponder
{

   public:
      CPlayerView *playerView;

      struct SEnemy
      {
         CCollisionActor * Actor;
         ISceneObject * Renderable;
         int KillMethod;
      };

      typedef std::vector<CBadGuy*> EnemyList;
      EnemyList Enemies, KillList;

      std::vector<EKiwi*> KiwiList;

      typedef std::vector<CItem*> ItemList;
      ItemList Items, KillItemList;

      typedef std::vector<CFriends*> FriendList;
      FriendList Friends;

   private:
      CParticleEngine * playerDeathParticleEngine; 
      int won, dead, gameOver;
      CCollisionActor * PlayerActor;
      CCollisionObject * VictoryFlag;
      CCollisionObject * SecretVictoryFlag;
      int PlayerHealth, PlayerEnergy;
      float PlayerRecovering;

      CCollisionEngine * Engine;

      CGameEventManager * GameEventManager;

   public:
      CGameplayManager(CCollisionActor * playerActor, CCollisionEngine * engine);

      static int PlayerLives;
      static int SeedCount;
      static void setLives(int num);
      static int const getPlayerLives();
      static int const getNumLives();
      static void subPlayerLife() {PlayerLives--;}

      bool OnCollision(CCollideable * Object, CCollideable * With);
      void runDeathSequence(float);
      void runVictorySequence(float);
      void Clear(CCollisionActor * playerActor, CCollisionEngine * engine);
      void Clear();

      int ShootingLaser;
      SRect2 LaserBox;
      int GodMode;
      float GodModeTime;
      float JustKilled;
      void setGodMode(float time);
      bool const isPlayerAlive() const;
      bool const isWon() const;
      bool const isGameOver() const;

      int const getPlayerHealth() const;
      int const getPlayerEnergy() const;
      static int const getSeedCount();

      void UseAbility(int energyCost);
      int const DamagePlayer(int damageTaken, CBadGuy* attacker, CCollideable* Other);
      float getRecovering() {return PlayerRecovering;}
      void setRecovering(float val) {PlayerRecovering = val;}

      bool const isJumping() const;

      SVector2f getPlayerLocation();

      CCollisionEngine* getEngine();

      void run(float const TickTime);

      CGameEventManager & getGameEventManager();

      void setVictoryFlag(CCollisionObject * f);
      void setSecretVictoryFlag(CCollisionObject * f);
};

#endif
