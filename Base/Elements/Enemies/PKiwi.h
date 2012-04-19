#ifndef PKIWI_H_
#define PKIWI_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class PKiwi : public CBadGuy
{

public:
   PKiwi(float x, float y, float w, float h, CGameplayManager* manager);

   void update(const float TickTime);
   void loadMesh();
   void loadActor();
   //void doRenderable();
};

#endif /* PKIWI_H_ */
