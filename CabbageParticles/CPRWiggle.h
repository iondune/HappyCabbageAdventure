#ifndef ____CPRWIGGLE_H_INCLUDE______________
#define ____CPRWIGGLE_H_INCLUDE______________

#include "CParticle.h"
#include "CParticleRiser.h"

class CPRWiggle : public CParticleRiser {
   float zValue, xValue, xWiggle;

   public:
   void setupRenderable();
   void updateMatrices(float);

   CPRWiggle();
};

#endif
