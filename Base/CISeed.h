#ifndef ___CISEED__H_
#define ___CISEED__H_

#include "CItem.h"
#include "CGameplayManager.h"

class CGameplayManager;
class CItem;

class CISeed : public CItem
{

public:
   CISeed(float x, float y, float w, float h, CGameplayManager* manager);

   float rotate;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float Time;
};

#endif /* ___CISeed__H_ */
