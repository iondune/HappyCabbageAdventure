#ifndef CELEMENTENEMYPomegranite_H_
#define CELEMENTENEMYPomegranite_H_

#include "CElementEnemy.h"

class CElementEnemyPomegranite : public CElementEnemy, ISquishable
{
   private:
      CParticleEngine *particleEngine;
      float ScaleMult, FlameTimer;
      bool PositiveScale, HitPlayer;
      SVector2 Scale;
      float OldPositionX;


   public:
      CElementEnemyPomegranite(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void OnCollision(CCollideable *Object);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();

      void HideFlame();
};

#endif
