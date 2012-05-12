#include "CPBurst.h"

#define RAND_POS_NEG(x) ((float)rand()/(float)RAND_MAX*2*(x) - (x))

void CPBurst::setupRenderable() {
   zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.4f + 0.1f);
   zFactor = 0;

   BlinkFreq = 0.0f; 
   //BlinkFreq = (float)rand()/(float)RAND_MAX*2.6f + 15.2f;
   BlinkTime = (float)rand()/(float)RAND_MAX*0.6f + 2.2f;
   Counter = 0;
}

void CPBurst::setEngine(CCollisionEngine *engine) {
   Engine = engine;
   Actor = Engine->addActor();
   Actor->setTypeId(INTERACTOR_NONCOLLIDERS);
   Actor->setCollisionMask(INTERACTOR_BLOCKS);
   Actor->setArea(SRect2f(centerPos->X, centerPos->Y, 0.001f, 0.001f));
   //Actor->setArea(SRect2f(centerPos->X + RAND_POS_NEG(0.3f), centerPos->Y + RAND_POS_NEG(0.3f), 0.05, 0.05));
   /*
   Actor->addImpulse(SVector2f(
            (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX*2.6f + 0.8f),
            (float)rand()/(float)RAND_MAX*3.6f + 0.6f), 0.1f);
   */
   Actor->addImpulse(SVector2f(
            ((float)rand()/(float)RAND_MAX*4.6f - 4.6f/2.0f),
            (float)rand()/(float)RAND_MAX*3.6f + 0.6f), 0.1f);
   Actor->Gravity = 30.0f;
   Actor->getAttributes().Bounce = (float)rand()/(float)RAND_MAX*2.6f + 1.2f;
}

void CPBurst::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else {
      Duration += timeElapsed;
      zFactor += zVelocity * (float) Actor->getVelocity().X * timeElapsed;
      if(Duration >= BlinkTime && BlinkFreq >= -0.8f) {
         BlinkFreq -= timeElapsed*1.0f;
      }
      translate = SVector3f(Actor->getArea().getCenter() + 0.1f, zFactor);
      translate += SVector3f(0.0f, BlinkFreq, 0.0f);
   }
}
