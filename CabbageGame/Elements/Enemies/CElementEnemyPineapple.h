#ifndef CELEMENTENEMYPinePineapple_H_
#define CELEMENTENEMYPinePineapple_H_

#include "CElementEnemy.h"

class CElementEnemyPineapple : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult;
      bool PositiveScale;
      SVector2 Scale;

   public:
      CElementEnemyPineapple(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
