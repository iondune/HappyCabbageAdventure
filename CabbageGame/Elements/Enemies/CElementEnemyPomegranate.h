#ifndef CELEMENTENEMYPomegranate_H_
#define CELEMENTENEMYPomegranate_H_

#include "CElementEnemy.h"

class CElementEnemyPomegranate : public CElementEnemy, ISquishable
{
   private:
      CParticleEngine *particleEngine;
      float ScaleMult, FlameTimer;
      bool PositiveScale, HitPlayer;
      SVector2f Scale;
      float OldPositionX;


   public:
      CElementEnemyPomegranate(SRect2f nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void OnCollision(const SCollisionEvent& Event);

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();
      void die();

      void HideFlame();
};

#endif
