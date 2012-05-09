#ifndef CELEMENTENEMYPomegranate_H_
#define CELEMENTENEMYPomegranate_H_

#include "CElementEnemy.h"

class CElementEnemyPomegranate : public CElementEnemy, ISquishable
{
   private:
      CParticleEngine *particleEngine;
      float ScaleMult, FlameTimer;
      bool PositiveScale, HitPlayer;
      SVector2 Scale;
      float OldPositionX;


   public:
      CElementEnemyPomegranate(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void OnCollision(CCollideable *Object);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();

      void HideFlame();
};

#endif
