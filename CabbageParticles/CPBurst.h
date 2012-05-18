#ifndef ____CPBURST_H_INCLUDE______________
#define ____CPBURST_H_INCLUDE______________

#include "CParticle.h"
#include "CPhysicsParticle.h"
#include "CCollisionEngine.h"

class CPBurst : public CPhysicsParticle {
   float zFactor, zVelocity, zAcceleration;
   float BlinkFreq, BlinkTime;
   float Size;

   void setupRenderable();
   void updateMatrices(float);
   public:
      void setEngine(CCollisionEngine *engine, int env);
};

#endif
