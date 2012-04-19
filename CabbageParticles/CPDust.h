#ifndef ____CPDUST_H_INCLUDE______________
#define ____CPDUST_H_INCLUDE______________

#include "CParticle.h"
#include "CPhysicsParticle.h"
#include "CCollisionEngine.h"

class CPDust : public CPhysicsParticle {
   float zFactor, zVelocity, zAcceleration;
   float Size;
   int ready;

   void setupActor();
   void setupRenderable();
   void updateMatrices(float);
   public:
      void setEngine(CCollisionEngine *engine);
};

#endif
