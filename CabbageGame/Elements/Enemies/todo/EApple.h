#ifndef EAPPLE_H_
#define EAPPLE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EApple : public CBadGuy
{

public:
   EApple(float x, float y, float w, float h, CGameplayManager* manager, int env);

   float rotate;
   bool rollingLeft, rollingRight;
   int env;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float ScaleMult;
   float Time;
   bool PositiveScale;
};

#endif /* EAPPLE_H_ */
