#ifndef ____CPBURST_H_INCLUDE______________
#define ____CPBURST_H_INCLUDE______________

#include "CParticle.h"
#include "CPhysicsParticle.h"
#include "CEngine.h"

class CPBurst : public CPhysicsParticle {
   float zFactor, zVelocity, zAcceleration;
   float Size;

   void setupRenderable();
   void updateMatrices(float);
   public:
      void setEngine(Cabbage::Collider::CEngine *engine);
};

#endif
