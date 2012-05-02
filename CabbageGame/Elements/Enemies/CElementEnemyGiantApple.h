#ifndef CELEMENTENEMYGIANTAPPLE_H_
#define CELEMENTENEMYGIANTAPPLE_H_

#include "CElementEnemy.h"

class CElementEnemyGiantApple : public CElementEnemy, public ISquishable
{
   private:
      float ScaleMult;
      bool PositiveScale;
      SVector2 Scale;

   public:
      CElementEnemyGiantApple(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
