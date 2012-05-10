#ifndef CELEMENTENEMYPinePineapple_H_
#define CELEMENTENEMYPinePineapple_H_

#include "CElementEnemy.h"

class CElementEnemyPineapple : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult, OldPositionX;
      bool PositiveScale, HitPlayer;
      SVector2 Scale;

   public:
      CElementEnemyPineapple(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void OnCollision(CCollideable *Object);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
