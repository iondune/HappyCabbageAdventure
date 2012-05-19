#ifndef CELEMENTENEMYStrawberry_H_
#define CELEMENTENEMYStrawberry_H_

#include "CElementEnemy.h"


namespace Strawberry {
   enum JumpStatus {
      NO_JUMP = 0,
      FIRST_JUMP,
      SECOND_JUMP,
      THIRD_JUMP,
      FOURTH_JUMP,
      FIFTH_JUMP,
      SIXTH_JUMP
   };
}

class CElementEnemyStrawberry : public CElementEnemy, public ISquishable
{
   private:
      SVector2f Scale;
      float OldPositionX, HitPlayer, JumpTimer;
      int JumpNum;

   public:
      CElementEnemyStrawberry(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      //Chris, this isn't even overriding CElementEnemy's OnCollision function.
      //virtual void OnCollision(CCollideable * Object);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
