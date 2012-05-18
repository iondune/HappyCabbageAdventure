#include "CPLeaf.h"

void CPLeaf::setupRenderable() {
   sineValue = 0;

   yFactor = (float)rand()/(float)RAND_MAX * 0.6f - 0.3f;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.5f + 0.3f;
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
}

void CPLeaf::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else {
      sineValue += 4*timeElapsed;
      translate = SVector3f(Amplitude*cos(Period*sineValue),
                           yFactor,
                           Amplitude*sin(Period*sineValue));
   }
}
