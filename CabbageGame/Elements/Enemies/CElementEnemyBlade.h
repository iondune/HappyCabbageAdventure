#ifndef CELEMENTENEMYBLADE_H_
#define CELEMENTENEMYBLADE_H_

#include "CElementEnemy.h"

class CElementEnemyBlade : public CElementEnemy
{
   private:
      float BladeRotate;

   public:
      CElementEnemyBlade(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
      
      virtual void OnCollision(CCollideable*);

      virtual void printInformation();
};

#endif
