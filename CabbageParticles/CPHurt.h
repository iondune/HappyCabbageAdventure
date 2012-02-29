#ifndef ____CPHURT_H_INCLUDE______________
#define ____CPHURT_H_INCLUDE______________

#include "CParticle.h"

class CPHurt : public CParticle {
   float tickValue;
   SVector3 position, velocity;
   void setupRenderable();
   void setCenterPos(SVector3*);
   void updateMatrices(float);
};

#endif
