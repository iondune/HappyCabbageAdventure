#ifndef EAPPLE_H_
#define EAPPLE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EApple : public CBadGuy
{

public:
   EApple(float x, float y, float w, float h, CGameplayManager* manager);

   float rotate;
   bool rollingLeft, rollingRight;

   void update();
   void loadMesh();
   void loadActor();
};

#endif /* EAPPLE_H_ */
