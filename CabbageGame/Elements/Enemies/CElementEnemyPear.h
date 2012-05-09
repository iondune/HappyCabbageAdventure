#ifndef CELEMENTENEMYPear_H_
#define CELEMENTENEMYPear_H_

#include "CElementEnemy.h"

class CElementEnemyPear : public CElementEnemy, public ISquishable
{
   private:
      SVector2 Scale;
      float OldPositionX;

   public:
      CElementEnemyPear(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
