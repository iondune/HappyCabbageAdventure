#ifndef ____CPLASER2_H_INCLUDE______________
#define ____CPLASER2_H_INCLUDE______________

#include "CParticle.h"

class CPLaser2 : public CParticle {
   float tickValue, xValue, yValue, zValue;
   void setupRenderable();
   void updateMatrices(float);
};

#endif
