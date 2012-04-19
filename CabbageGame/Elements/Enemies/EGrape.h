#ifndef EGRAPE_H_
#define EGrape_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EGrape : public CBadGuy
{

public:
   EGrape(float x, float y, float w, float h, CGameplayManager* manager);

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   void ThrowGrape();

   float Time;
};

#endif /* EGrape_H_ */
