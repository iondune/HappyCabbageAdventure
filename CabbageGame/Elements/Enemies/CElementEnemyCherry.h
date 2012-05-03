#ifndef CELEMENTENEMYCHERRY_H_
#define CELEMENTENEMYCHERRY_H_

#include "CElementEnemy.h"

class CElementEnemyCherry : public CElementEnemy, public ISquishable
{
   private:
      SVector2 Scale;

   public:
      CElementEnemyCherry(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
