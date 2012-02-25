#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "CPlayerView.h"

#include "EApple.h"

class CGameEventManager;
class CRenderable;
class EApple;


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

   private:
      CParticleEngine * playerDeathParticleEngine; 
      int won, dead;
      Cabbage::Collider::CActor * PlayerActor;
      Cabbage::Collider::CObject * VictoryFlag;
      int PlayerHealth, PlayerEnergy;
      float PlayerRecovering;

      Cabbage::Collider::CEngine * Engine;

      CGameEventManager * GameEventManager;

   public:
      CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine);

      void OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With);
      void runDeathSequence(float);

      int GodMode;
      float GodModeTime;
      void setGodMode(float time);
      bool const isPlayerAlive() const;
      bool const isWon() const;

      int const getPlayerHealth() const;
      int const getPlayerEnergy() const;

      void UseAbility(int energyCost);
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
