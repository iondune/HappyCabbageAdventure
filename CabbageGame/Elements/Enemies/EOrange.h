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
