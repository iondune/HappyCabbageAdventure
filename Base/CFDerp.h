#ifndef ___CFDERP__H_
#define ___CFDERP__H_

#include "CFriends.h"
#include "CGameplayManager.h"

class CGameplayManager;
class CFriends;

class CFDerp : public CFriends

{

public:
   CFDerp(float x, float y, float w, float h, CGameplayManager* manager);

   float rotate;

   void update(const float TickTime);
   void loadMesh();
   void loadActor();

   void doRenderable();

   float Time;
};

#endif /* ___CIHEALTH__H_ */