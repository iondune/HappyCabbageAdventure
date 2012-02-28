#include "CPLaser2.h"

void CPLaser2::setupRenderable() {
   tickValue = 0;
   yValue = (float)rand()/(float)RAND_MAX * 1 - 0.5f;
   xValue = (float)rand()/(float)RAND_MAX * 20 - 10;
   zValue = (float)rand()/(float)RAND_MAX * 1 - 0.5f;
   

   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   Counter = StartFactor;

}

void CPLaser2::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration >= TotalDuration - (float)AppearRate*0.85) {
      //CApplication::get().getSceneManager().removeSceneObject(getRenderable());
      Duration = -1;
   }
   else if(Duration == -1) {}
   else {
      tickValue += 4*timeElapsed;
      Duration += timeElapsed;

      xValue += 20*timeElapsed;
      if(xValue < 0 || xValue > 5) {
         translate = SVector3(-50);
         if(xValue > 5)
            xValue -= 6;
      }
      else {
         translate = SVector3((*lookRight?1:-1)*xValue,
               yValue * ((xValue > 1 || xValue <= 0) ? 1 : xValue),
               zValue);
      }

      //getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
