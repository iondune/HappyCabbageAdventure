#ifndef CPLAYERABILITY__HEADER
#define CPLAYERABILITY__HEADER

#include "IUpdater.h"

class CElementPlayer;
class CCollideable;

namespace Abilities {
   enum EAbilityType {
      SHIELD = 0,
      LASERCHARGE = 1,
      LASERFIRE = 2,
      DASH = 3,
      BLINK = 4
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
      CPlayerAbility(CElementPlayer &, Abilities::EAbilityType);
      Abilities::EAbilityType getType();
};

#endif
