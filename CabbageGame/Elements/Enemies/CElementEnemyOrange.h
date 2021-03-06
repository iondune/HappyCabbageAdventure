#ifndef CELEMENTENEMYORANGE_H_
#define CELEMENTENEMYORANGE_H_

#include "CElementEnemy.h"
#include "../ISquishable.h"

class CElementEnemyOrange : public CElementEnemy, public ISquishable
{
   private:
      bool Jumped;
      SVector2f Scale;

   public:
      CElementEnemyOrange(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
};

#endif
