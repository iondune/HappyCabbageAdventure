#ifndef EORANGE_H_
#define EORANGE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class EOrange : public CBadGuy
{

public:
   EOrange(float x, float y, float w, float h, CGameplayManager* manager);

   bool jump;
   double oldX;

   void update(float const TickTime);
   void loadMesh();
   void loadActor();
};

#endif /* EORANGE_H_ */
