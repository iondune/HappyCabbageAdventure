#ifndef CPLAYERABILITY__HEADER
#define CPLAYERABILITY__HEADER

#include "IUpdater.h"

class CElementPlayer;
class CCollideable;

class CPlayerAbility {
   CElementPlayer & Player;
   float ElapsedTime;

   public:

      virtual void inUpdatePhysicsEngine(float)=0;
      virtual void inUpdateSceneObject(float)=0;
      virtual void inOnCollision(CCollideable *)=0;
      CPlayerAbility(CElementPlayer &);
};

#endif
