#ifndef __CPLAYER_H____
#define __CPLAYER_H____

#include "../CGameplayElement.h"
#include "CEventManager.h"
#include "../ISquishable.h"
#include "CPlayerAbility.h"
#include "sound.h"

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
   friend class CPlayerAbilityDash;
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
      CParticleEngine *glow;
      ISceneObject * SceneObject; //Override CGameplayElement's. This will contain the cabbage and its shadow 
      CPlayerView *View;

      Cabbage::PlayerInformation Stats;
      EDirection Direction;
      EAction Action; 

      bool AllowMovement, Victory;

      void updatePlayerAction();
      float Recovering, Shaking, ShakeFactorFactor, VictoryTime;
      float MoveKeyDelay;
      SVector2 Scale;
      SVector3 ShakeFactor;

      //Sound Variables
      Mix_Chunk *takeDmg;
      Mix_Chunk *jump;
      Mix_Chunk *chargeLaser1;
      Mix_Chunk *chargeLaser2;
      Mix_Chunk *fireLaser;
      bool PlayJump;

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
      void incrementLives();
      void decrementLives();
      void changeEnergy(int amount);
      void incrementSeeds();
      CPlayerAbility *getAbility(Abilities::EAbilityType a);

      void setShaking(float, float);
      void setupSoundEffects();

      void playLevelVictory(float time);
      void setVictoryFlag(bool value);

      void setAllowMovement(bool value);

      //Keyboard event functions
      void OnKeyboardEvent(SKeyboardEvent const & Event);
      Cabbage::PlayerInformation & getStats();
};

#endif
