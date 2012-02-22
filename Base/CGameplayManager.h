#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"

#include "EApple.h"

class CGameEventManager;
class CRenderable;
class EApple;


class CGameplayManager : public Cabbage::Collider::ICollisionResponder
{

   public:

      struct SEnemy
      {
         Cabbage::Collider::CActor * Actor;
         ISceneObject * Renderable;
      };

      typedef std::vector<CBadGuy*> EnemyList;
      EnemyList Enemies, KillList;

   private:

      int won;
      Cabbage::Collider::CActor * PlayerActor;
      Cabbage::Collider::CObject * VictoryFlag;
      int PlayerHealth;
      float PlayerRecovering;

      Cabbage::Collider::CEngine * Engine;

      CGameEventManager * GameEventManager;

   public:
      CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine);

      void OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With);

      bool const isPlayerAlive() const;
      bool const isWon() const;

      int const getPlayerHealth() const;

      float getRecovering() {return PlayerRecovering;}

      bool const isJumping() const;

      SVector2 getPlayerLocation();

      Cabbage::Collider::CEngine* getEngine();

      void run(float const TickTime);

      CGameEventManager & getGameEventManager();

      void setVictoryFlag(Cabbage::Collider::CObject * f);

};

#endif
