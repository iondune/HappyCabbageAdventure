#ifndef ___CIHEALTH__H_
#define ___CIHEALTH__H_

#include "CItem.h"
#include "CGameplayManager.h"

class CGameplayManager;
class CItem;

class CIHealth : public CItem
{

public:
   CIHealth(float x, float y, float w, float h, CGameplayManager* manager);

   float rotate;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float Time;
};

#endif /* ___CIHEALTH__H_ */
