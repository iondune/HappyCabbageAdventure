#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "CPlayerView.h"

#include "CItem.h"
#include "CIHealth.h"
#include "EApple.h"

class CGameEventManager;
class CRenderable;
class EApple;
class CItem;
class CIHealth;


class CGameplayManager : public Cabbage::Collider::ICollisionResponder
{

   public:
      CPlayerView *playerView;

      struct SEnemy
      {
         Cabbage::Collider::CActor * Actor;
         ISceneObject * Renderable;
      };

      typedef std::vector<CBadGuy*> EnemyList;
      EnemyList Enemies, KillList;

      typedef std::vector<CItem*> ItemList;
      ItemList Items, KillItemList;

   private:
      CParticleEngine * playerDeathParticleEngine; 
      int won, dead, gameOver;
      Cabbage::Collider::CActor * PlayerActor;
      Cabbage::Collider::CObject * VictoryFlag;
      int PlayerHealth, PlayerEnergy, PlayerLives;
      float PlayerRecovering;

      Cabbage::Collider::CEngine * Engine;

      CGameEventManager * GameEventManager;

   public:
      CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine);

      void OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With);
      void runDeathSequence(float);

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
      int const getPlayerLives() const;

      void UseAbility(int energyCost);
      int const DamagePlayer(int damageTaken, CBadGuy* attacker, Cabbage::Collider::CCollideable* Other);
      float getRecovering() {return PlayerRecovering;}
      void setRecovering(float val) {PlayerRecovering = val;}

      bool const isJumping() const;

      SVector2 getPlayerLocation();

      Cabbage::Collider::CEngine* getEngine();

      void run(float const TickTime);

      CGameEventManager & getGameEventManager();

      void setVictoryFlag(Cabbage::Collider::CObject * f);

};

#endif
