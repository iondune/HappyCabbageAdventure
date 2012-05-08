#ifndef CELEMENTENEMYPomegranite_H_
#define CELEMENTENEMYPomegranite_H_

#include "CElementEnemy.h"

class CElementEnemyPomegranite : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult;
      bool PositiveScale;
      SVector2 Scale;

   public:
      CElementEnemyPomegranite(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
