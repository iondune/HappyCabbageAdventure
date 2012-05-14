#ifndef CELEMENTENEMYLemon_H_
#define CELEMENTENEMYLemon_H_

#include "CElementEnemy.h"

class CElementEnemyLemon : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult;
      bool PositiveScale;
      SVector2 Scale;

   public:
      CElementEnemyLemon(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual int takeDamage(int amount);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();

      void explode();
};

#endif
