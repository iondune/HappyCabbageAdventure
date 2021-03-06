#include "CPDeath.h"

void CPDeath::setupRenderable() {
   sineValue = 0;

   yFactor = (float)rand()/(float)RAND_MAX * 0.6f - 0.3f;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.5f + 0.3f;
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
}

void CPDeath::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration >= TotalDuration - (float)AppearRate*0.85) {
      //CApplication::get().getSceneManager().removeSceneObject(getRenderable());
      Duration = -1;
   }
   else if(Duration == -1) {}
   else {
      sineValue += 4*timeElapsed;
      Duration += timeElapsed;
      yFactor += timeElapsed;
      translate = SVector3f(Amplitude*cos(Period*sineValue),
                           yFactor,
                           Amplitude*sin(Period*sineValue));
   }
}
