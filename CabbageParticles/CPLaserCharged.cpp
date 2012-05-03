#include "CPLaserCharged.h"

void CPLaserCharged::setupRenderable() {
   xFactor = (float)rand()/(float)RAND_MAX * 360.0f;
   yHeight = 0.0f;
}

void CPLaserCharged::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else {
      yHeight += timeElapsed*0.8f;
      xFactor -= timeElapsed*8.0f;
      if(yHeight > 0.9f)
         yHeight = 0.0f;
      translate = SVector3(
            0.55f*sin(xFactor),
            yHeight,
            0.55f*cos(xFactor));
   }
}
