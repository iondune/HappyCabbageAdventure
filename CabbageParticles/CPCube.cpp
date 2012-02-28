#include "CPCube.h"

void CPCube::setupRenderable() {
   sineValue = 0;

   yFactor = (float)rand()/(float)RAND_MAX * 1.5f - 0.2f;
   Amplitude = 0.4f*(yFactor + 1);
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   Counter = StartFactor;

   Size = (float)rand()/(float)RAND_MAX*0.8f + 0.2f;
}

void CPCube::updateMatrices(float timeElapsed) {
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
      float newX = (*lookRight?1:-1)*(0.8f+yFactor);
      translate = SVector3(newX,
                           Amplitude*cos(Period*sineValue), 
                           Amplitude*sin(Period*sineValue));
   }
}
