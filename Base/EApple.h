#ifndef EAPPLE_H_
#define EAPPLE_H_

#include "CBadGuy.h"

class EApple : public CBadGuy
{

public:
   EApple(float x, float y, float w, float h, CGameplayManager* manager);

   void update();
   void loadMesh();
   void loadActor();
};

#endif /* EAPPLE_H_ */
