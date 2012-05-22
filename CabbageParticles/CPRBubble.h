#ifndef ____CPRBUBBLE_H_INCLUDE______________
#define ____CPRBUBBLE_H_INCLUDE______________

#include "CParticle.h"
#include "CParticleRiser.h"

class CPRBubble : public CParticleRiser {
   float zValue, xValue, xWiggle;
   float baseXValue, baseYValue, baseZValue;

   public:
   void setupRenderable();
   void updateMatrices(float);

   CPRBubble();
};

#endif
