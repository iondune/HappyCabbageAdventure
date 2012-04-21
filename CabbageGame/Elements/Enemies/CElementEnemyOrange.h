#ifndef CELEMENTENEMYAPPLE_H_
#define CELEMENTENEMYAPPLE_H_

#include "CElementEnemy.h"

class CElementEnemyApple : public CElementEnemy
{
   public:
      enum RollDirection {
         Left,
         Right,
         None
      };
   private:
      RollDirection Roll;
      float Rotate, ScaleMult;
      bool PositiveScale;

   public:
      CElementEnemyApple(SRect2 nArea);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(CCollideable *Object, CCollideable *With);
};

#endif
#ifndef EORANGE_H_
#define EORANGE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EOrange : public CBadGuy
{

public:
   EOrange(float x, float y, float w, float h, CGameplayManager* manager, int env);

   bool jump;
   double oldX;
   int env;

   void update(float const TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();
};

#endif /* EORANGE_H_ */
