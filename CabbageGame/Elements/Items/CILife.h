#ifndef ___CILIFE__H_
#define ___CILIFE__H_

#include "CItem.h"
#include "CGameplayManager.h"

class CGameplayManager;
class CItem;

class CILife : public CItem
{

public:
   CILife(float x, float y, float w, float h, CGameplayManager* manager);

   float rotate;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float Time;
};

#endif /* ___CIHEALTH__H_ */