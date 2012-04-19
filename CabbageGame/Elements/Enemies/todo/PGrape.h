#ifndef PGRAPE_H_
#define PGRAPE_H_

#include "CBadGuy.h"
#include "CGameplayManager.h"

class PGrape : public CBadGuy
{

public:
   PGrape(float x, float y, float w, float h, CGameplayManager* manager);

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   bool Direction;
   int r;
};

#endif /* PGRAPE_H_ */
