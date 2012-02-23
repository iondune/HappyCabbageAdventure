#ifndef EBLADE_H_
#define EBLADE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EBlade : public CBadGuy
{

public:
   EBlade(float x, float y, float w, float h, CGameplayManager* manager);

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float BladeRotate;
};

#endif /* EKIWI_H_ */
