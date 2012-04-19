#ifndef _______CPHYSICSPARTICLE___H__INCLUDE___
#define _______CPHYSICSPARTICLE___H__INCLUDE___

#include "CParticle.h"
#include "CEngine.h"

class CPhysicsParticle : public CParticle {

   virtual void setupRenderable() =0;
   virtual void updateMatrices(float) =0;

   public:
   Cabbage::Collider::CActor *Actor;
   Cabbage::Collider::CCollisionEngine *Engine;

   virtual void setEngine(Cabbage::Collider::CCollisionEngine *engine);
   virtual void deconstruct();
};

#endif
