#ifndef ____CPSTAR_H_INCLUDE______________
#define ____CPSTAR_H_INCLUDE______________

#include "CParticle.h"

class CPStar : public CParticle {
   float xValue, yValue, zValue;

   void setupRenderable();
   void updateMatrices(float);
};

#endif
