#include "CPRBubble.h"

CPRBubble::CPRBubble() : CParticleRiser(3.0f, 0.1f, 0.8f) {

}

void CPRBubble::setupRenderable() {
   CParticleRiser::setupRenderable();
   xValue = frand() * 1.5f - 0.75f;
   xWiggle = (float) (rand() % 360); 
   zValue = frand() * 0.2f - 0.1f;

   baseXValue = centerPos->X;
   baseYValue = centerPos->Y;
   baseZValue = centerPos->Z;
}

void CPRBubble::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
      if(Counter <= 0.0f) {
         baseYValue = centerPos->Y;
         baseXValue = centerPos->X;
         baseZValue = centerPos->Z;
      }
   }
   else {
      xWiggle += timeElapsed * 1.0f;
      yValue += ySpeed * timeElapsed;
      if(yValue > baseYValue + localMaxHeight) {
         yValue = 0.0f;
         baseYValue = centerPos->Y;
         baseXValue = centerPos->X;
         baseZValue = centerPos->Z;
      }
      translate = SVector3f((xValue + sin(xWiggle)*0.05f) * (yValue / localMaxHeight) + baseXValue, yValue + baseYValue, zValue + baseZValue);
   }
}
