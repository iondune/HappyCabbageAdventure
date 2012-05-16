#ifndef CELEMENTENEMYLemon_H_
#define CELEMENTENEMYLemon_H_

#include "CElementEnemy.h"

class CElementEnemyLemon : public CElementEnemy, ISquishable
{
   private:
      float ScaleMult;
      bool PositiveScale;
      SVector2f Scale;

   public:
      CElementEnemyLemon(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual int takeDamage(int amount);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();

      void explode();
};

#endif
