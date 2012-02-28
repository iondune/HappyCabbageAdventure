#ifndef ____CPFLAME_H_INCLUDE______________
#define ____CPFLAME_H_INCLUDE______________

#include "CParticle.h"

class CPFlame : public CParticle {
   float yFactor, Amplitude, Period;
   float sineValue;
   float Size;

   float yVelocity, yHeight;
   float maxHeight;
   void setupRenderable();
   void updateMatrices(float);
};

#endif
