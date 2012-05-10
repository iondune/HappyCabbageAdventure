#ifndef CELEMENTENEMYStrawberry_H_
#define CELEMENTENEMYStrawberry_H_

#include "CElementEnemy.h"

class CElementEnemyStrawberry : public CElementEnemy, public ISquishable
{
   private:
      SVector2 Scale;

   public:
      CElementEnemyStrawberry(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
