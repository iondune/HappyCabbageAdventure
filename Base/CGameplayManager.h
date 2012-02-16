#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"

class CGameEventManager;
class CRenderable;

class CGameplayManager : public Cabbage::Collider::ICollisionResponder
{

   public:

      struct SEnemy
      {
         Cabbage::Collider::CActor * Actor;
         CRenderable * Renderable;
      };

      typedef std::vector<SEnemy> EnemyList; //Will change to vector<CBadGuy>
      EnemyList Enemies;
      EnemyList KillList;

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

      SVector2 getPlayerLocation();

      Cabbage::Collider::CEngine* getEngine();

      void run(float const TickTime);

      void addEnemy(SVector2 const & Position, CRenderable * renderable);

      CGameEventManager & getGameEventManager();

      void setVictoryFlag(Cabbage::Collider::CObject * f);

};

#endif
