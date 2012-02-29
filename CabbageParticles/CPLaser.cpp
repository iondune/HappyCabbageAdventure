#include "CPLaser.h"

void CPLaser::setupRenderable() {
   sineValue = 0;

   xFactor = (float)rand()/(float)RAND_MAX * 3 - 1.5f;
   yFactor = (float)rand()/(float)RAND_MAX * 3 - 1.5f;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.7f + 0.2; 
   zFactor = (float)rand()/(float)RAND_MAX * 2 - 1;
   reflectX = rand() % 2 * 2 - 1;
   reflectY = rand() % 2 * 2 - 1;

   Size = (float)rand()/(float)RAND_MAX*0.8f + 0.2f;
}

void CPLaser::updateMatrices(float timeElapsed) {
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
      Amplitude = Amplitude + (Amplitude < 0 ? 1 : -1)*1.5f*timeElapsed; 
      if(Amplitude <= 0.01 && Amplitude >= -0.01)
         Amplitude = 0;
      Duration += timeElapsed;
      translate = SVector3((*lookRight?1:-1)*reflectX*Amplitude*cos(xFactor),
                           Amplitude*sin(yFactor),
                           Amplitude*zFactor);
      //getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
