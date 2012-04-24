#ifndef CELEMENTENEMYAPPLE_H_
#define CELEMENTENEMYAPPLE_H_

#include "CElementEnemy.h"

class CElementEnemyApple : public CElementEnemy, public ISquishable
{
   public:
      enum RollDirection {
         Left,
         Right,
         None
      };
   private:
      RollDirection Roll;
      float Rotate, ScaleMult;
      bool PositiveScale;
      SVector2 Scale;

   public:
      CElementEnemyApple(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
