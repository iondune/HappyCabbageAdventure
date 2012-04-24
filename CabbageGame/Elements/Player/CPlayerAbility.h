#ifndef CPLAYERABILITY__HEADER
#define CPLAYERABILITY__HEADER

#include "IUpdater.h"

class CElementPlayer;

class CPlayerAbility {
   CElementPlayer & Player;
   float ElapsedTime;

   public:
      virtual void inUpdatePhysicsEngine()=0;
      virtual void inUpdateSceneObject()=0;
      CPlayerAbility(CElementPlayer &);
};

#endif
