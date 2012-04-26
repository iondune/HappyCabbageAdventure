#ifndef __CPLAYER_H____
#define __CPLAYER_H____

#include "../CGameplayElement.h"
#include "CEventManager.h"
#include "../ISquishable.h"
#include "CPlayerAbility.h"

namespace Cabbage {
   //Information kept between levels
   class PlayerInformation {
      public:
         int Health;
         int MaxHealth;
         int Energy;
         int MaxEnergy;
         int Lives;
         int Seeds;

         PlayerInformation() : Health(5), MaxHealth(5), Energy(100), MaxEnergy(100),
         Lives(3), Seeds(0) {
         }
   };

   //Information kept in the Overworld
   class LevelInformation {
      public:
         int LevelsCompleted;
         int LastLevelEntered;

         LevelInformation() : LevelsCompleted(0), LastLevelEntered(0) {

         }
   };
}

class CPlayerView;

class CElementPlayer : public CGameplayElement, public ISquishable {
   friend class CPlayerAbility;
   friend class CPlayerAbilityShield;
   friend class CPlayerAbilityBlink;
   friend class CPlayerAbilityLaser;
   public:
      enum EDirection {
         Left = 0,
         Right = 1
      };
      enum EAction {
         Standing = 0,
         Walking = 1,
         Jumping = 2
      };

   private:
      CCollisionActor * PhysicsEngineObject; //Override CGameplayElement's but with type CObject
      ISceneObject * SceneObject; //Override CGameplayElement's. This will contain the cabbage and its shadow 
      CPlayerView *View;

      Cabbage::PlayerInformation Stats;
      EDirection Direction;
      EAction Action; 

      bool AllowMovement;

      void updatePlayerAction();
      float Recovering, Shaking, ShakeFactorFactor;
      SVector2 Scale;
      SVector3 ShakeFactor;

      std::vector<CPlayerAbility*> Abilities;
      void checkAbilityKeypress();
      void updateAbilities(float time);
      std::map<Abilities::EAbilityType, int> usedAbility;

   public:
      CElementPlayer(SRect2 nArea);

      //CGameplayElement functions
      virtual void OnCollision(CCollideable *Object);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float);
      virtual void updateSceneObject(float);

      virtual void writeXML(xmlwriter *);

      std::map<Abilities::EAbilityType, int> &getAbilityStatus();
      bool used(Abilities::EAbilityType a);

      bool decrementHealth();
      void incrementHealth();
      void incrementSeeds();
      CPlayerAbility *getAbility(Abilities::EAbilityType a);

      void setShaking(float, float);

      //Keyboard event functions
      void OnKeyboardEvent(SKeyboardEvent const & Event);
      Cabbage::PlayerInformation & getStats();
};

#endif
