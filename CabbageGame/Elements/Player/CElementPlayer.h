#ifndef __CPLAYER_H____
#define __CPLAYER_H____

#include "../CGameplayElement.h"
#include "CEventManager.h"
#include "../ISquishable.h"
#include "sound.h"
class CPlayerAbility;

#include "CabbageGameInformation.h"

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
      Mix_Music *victoryMusic;
      Mix_Music *deathMusic;
      bool PlayJump, UseCamera;

      std::vector<CPlayerAbility*> Abilities;
      void checkAbilityKeypress();
      void updateAbilities(float time);
      std::map<Abilities::EAbilityType, int> usedAbility;

   public:
      CElementPlayer(SRect2 nArea, bool useCamera = true);

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
      void setHealth(int amount);
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
      void setCanUseAbility(Abilities::EAbilityType t);
      void setStats(Cabbage::PlayerInformation st);
      EAction getAction();
};
#endif
