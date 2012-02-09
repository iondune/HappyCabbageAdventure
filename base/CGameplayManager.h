#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"
#include "sound.h"

class CGameEventManager;
class CRenderable;

class CGameplayManager : public Cabbage::Collider::ICollisionResponder
{

   public:

      struct SEnemy
      {
         Cabbage::Collider::CActor * Actor;
         CRenderable * Renderable;
         /*Enemy type variable here?*/
      };

      typedef std::vector<SEnemy> EnemyList;
      EnemyList Enemies;
      EnemyList KillList;

   private:

      int won;
      Cabbage::Collider::CActor * PlayerActor;
      Cabbage::Collider::CObject * Flag;
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

      void run(float const TickTime);

      void addEnemy(SVector2 const & Position, CRenderable * renderable);

      CGameEventManager & getGameEventManager();

      void setFlag(CabbageCollider::CObject * f) {
         Flag = f;
      }
};

#endif
