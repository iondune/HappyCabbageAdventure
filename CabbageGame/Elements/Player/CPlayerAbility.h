#ifndef CPLAYERABILITY__HEADER
#define CPLAYERABILITY__HEADER

#include "IUpdater.h"
#include "CCollisionEngine.h"

class CElementPlayer;
class CCollideable;

#include "CabbageGameInformation.h"

class CPlayerAbility {
   protected:
      CElementPlayer & Player;
      float ElapsedTime;
      bool Dead;
      Abilities::EAbilityType Type;

   public:
      virtual void inUpdatePhysicsEngineObject(float)=0;
      virtual void inUpdateSceneObject(float)=0;
      void updateTime(float);
      bool isDead();
      virtual void inOnCollision(const SCollisionEvent& Event);
      virtual void checkKey(bool keyDown);
      CPlayerAbility(CElementPlayer &, Abilities::EAbilityType);
      Abilities::EAbilityType getType();
};

#endif
