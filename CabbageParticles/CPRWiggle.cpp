#include "CPRWiggle.h"

CPRWiggle::CPRWiggle() : CParticleRiser(3.0f, 0.1f, 0.8f) {

}

void CPRWiggle::setupRenderable() {
   CParticleRiser::setupRenderable();
   xValue = frand() * 2.0f - 1.0f;
   xWiggle = rand() % 360; 
   zValue = frand() * 0.2f - 0.1f;
}

void CPRWiggle::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else {
      xWiggle += timeElapsed * 1.0f;
      CParticleRiser::updateMatrices(timeElapsed);
      translate = SVector3(xValue + sin(xWiggle)*0.05f, yValue, zValue);
   }
}
