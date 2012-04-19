#ifndef _______CPHYSICSPARTICLE___H__INCLUDE___
#define _______CPHYSICSPARTICLE___H__INCLUDE___

#include "CParticle.h"
#include "CCollisionEngine.h"

class CPhysicsParticle : public CParticle {

   virtual void setupRenderable() =0;
   virtual void updateMatrices(float) =0;

   public:
   CActor *Actor;
   CCollisionEngine *Engine;

   virtual void setEngine(CCollisionEngine *engine);
   virtual void deconstruct();
};

#endif
