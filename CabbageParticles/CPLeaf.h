#ifndef ____CPLEAF_H_INCLUDE______________
#define ____CPLEAF_H_INCLUDE______________

#include "CParticle.h"

class CPLeaf : public CParticle {
   float yFactor, Amplitude, Period;
   float sineValue;
   float Size;

   void setupRenderable();
   void updateMatrices(float);
};

#endif
