#ifndef ____CPHURT_H_INCLUDE______________
#define ____CPHURT_H_INCLUDE______________

#include "CParticle.h"

class CPHurt : public CParticle {
   float tickValue;
   SVector3f position, velocity;
   void setupRenderable();
   void setCenterPos(SVector3f*);
   void updateMatrices(float);
};

#endif
