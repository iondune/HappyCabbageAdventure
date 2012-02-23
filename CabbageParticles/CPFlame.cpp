#include "CPFlame.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

void CPFlame::setupRenderable() {
   sineValue = 0;

   yFactor = (float)rand()/(float)RAND_MAX * 1;
   yVelocity = (float)rand()/(float)RAND_MAX * 0.8 + 0.5;
   yHeight = 0;


   Period = (float)rand()/(float)RAND_MAX * 0.8 + 0.5;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.4 - 0.3;

   RotationSpeed = SVector3(
         (float)rand()/(float)RAND_MAX * 4 - 2, 
         (float)rand()/(float)RAND_MAX * 4 - 2,
         (float)rand()/(float)RAND_MAX * 4 - 2);
   RotationSpeed *= -yFactor + 2.0;

   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   Counter = StartFactor;

   Size = (float)rand()/(float)RAND_MAX*0.8 + 0.2;
   maxHeight = 1 + 0.1*cos(yFactor*5*M_PI);
}

#define MIN(x,y) ((x>y)?y:x)

void CPFlame::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
      translate = SVector3(-50);
   }
   else {
      sineValue += 4*timeElapsed;
      Duration += timeElapsed;

      yHeight += yVelocity*timeElapsed;

      if(yHeight > maxHeight) {
         yHeight = yVelocity*timeElapsed / 4;
      }

      // yFactor == [0, 1]
      // yHeight/maxHeight == [0, 1]
      
      translate = SVector3((yFactor - 0.5f) * MIN(1, pow(yHeight / maxHeight, 0.2f)) + 0.5f,
                           yHeight + 0.1f,
                           Amplitude*sin(Period*sineValue));
      //getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
