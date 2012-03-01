#ifndef ____CPBURST_H_INCLUDE______________
#define ____CPBURST_H_INCLUDE______________

#include "CParticle.h"
#include "CEngine.h"

class CPBurst : public CParticle {
   float zFactor, zVelocity, zAcceleration;
   float Size;

   Cabbage::Collider::CActor *Actor;
   Cabbage::Collider::CEngine *Engine;

   void setupRenderable();
   void updateMatrices(float);
   public:
      void setEngine(Cabbage::Collider::CEngine *engine);
};

#endif
