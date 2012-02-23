#ifndef EFLAME_H_
#define EFLAME_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EFlame : public CBadGuy
{

public:
   EFlame(float x, float y, float w, float h, CGameplayManager* manager);

   void update(const float TickTime);
   void loadMesh();
   void loadActor();
};

#endif /* EKIWI_H_ */
