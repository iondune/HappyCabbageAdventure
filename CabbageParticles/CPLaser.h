#ifndef ____CPLASER_H_INCLUDE______________
#define ____CPLASER_H_INCLUDE______________

#include "CParticle.h"

class CPLaser : public CParticle {
   float Amplitude, Period;
   float sineValue;
   float Size;

   float reflectX, reflectY, xFactor, yFactor, zFactor;
   void setupRenderable();
   void updateMatrices(float);
};

#endif
