#ifndef ____CPHURT_H_INCLUDE______________
#define ____CPHURT_H_INCLUDE______________

#include "CParticle.h"

class CPHurt : public CParticle {
   float tickValue, xValue, yValue, zValue;
   void setupRenderable();
   void updateMatrices(float);
};

#endif
