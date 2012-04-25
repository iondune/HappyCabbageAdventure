#ifndef CPLAYERABILITY__HEADER
#define CPLAYERABILITY__HEADER

#include "IUpdater.h"

class CElementPlayer;
class CCollideable;

namespace Abilities {
   enum EAbilityType {
      SHIELD = 0,
      LASER = 1,
      DASH = 2,
      BLINK = 3
   };
}

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
      virtual void inOnCollision(CCollideable *Object);
      virtual void checkKey(bool keyDown);
      CPlayerAbility(CElementPlayer &, Abilities::EAbilityType);
      Abilities::EAbilityType getType();
};

#endif
