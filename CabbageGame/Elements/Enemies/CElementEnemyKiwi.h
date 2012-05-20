#ifndef CELEMENTENEMYKIWI_H_
#define CELEMENTENEMYKIWI_H_

#include "CElementEnemy.h"

class CElementEnemyKiwi : public CElementEnemy
{
   private:
      float rotateBird, SineValue, OldX;
      bool bombDropped;
      int Direction;

   public:
      CElementEnemyKiwi(SRect2f nArea, int direction);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void printInformation();

      void OnPhaseBegin(const SCollisionEvent& Event);
      void OnPhaseEnd(const SCollisionEvent& Event);

      void DropBomb();
};

#endif
