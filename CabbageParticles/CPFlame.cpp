#include "CPFlame.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

void CPFlame::setupRenderable() {
   sineValue = 0;

   yFactor = (float)rand()/(float)RAND_MAX * 1;
   yVelocity = (float)rand()/(float)RAND_MAX * 0.8f + 0.5f;
   yHeight = 0;

   Period = (float)rand()/(float)RAND_MAX * 0.8f + 0.5f;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.4f - 0.3f;

   Size = (float)rand()/(float)RAND_MAX*0.8f + 0.2f;
   maxHeight = 1 + 0.1f*cos(yFactor*5*M_PI);
}

#define MIN(x,y) ((x>y)?y:x)

void CPFlame::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
      translate = SVector3f(-50);
   }
   else {
      sineValue += 4*timeElapsed;
      //Duration += timeElapsed;

      yHeight += yVelocity*timeElapsed;

      if(yHeight > maxHeight) {
         yHeight = yVelocity*timeElapsed / 4;
      }

      // yFactor == [0, 1]
      // yHeight/maxHeight == [0, 1]
      
      translate = SVector3f((yFactor - 0.5f) * MIN(1, pow(yHeight / maxHeight, 0.2f)) + 0.5f,
                           yHeight + 0.1f,
                           Amplitude*sin(Period*sineValue));
   }
}
