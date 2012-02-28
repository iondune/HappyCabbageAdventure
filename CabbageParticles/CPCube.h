#ifndef ____CPCUBE_H_INCLUDE______________
#define ____CPCUBE_H_INCLUDE______________

#include "CParticle.h"

class CPCube : public CParticle {
   float yFactor, Amplitude, Period;
   float sineValue;
   float Size;

   void setupRenderable();
   void updateMatrices(float);
};

#endif
