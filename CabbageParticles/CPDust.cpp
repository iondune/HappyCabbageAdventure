#include "CPDust.h"

#define RAND_POS_NEG(x) ((float)rand()/(float)RAND_MAX*2*(x) - (x))

void CPDust::setupRenderable() {
   ready = 0;
   zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.4f + 0.2f);
   zFactor = 0;
}

void CPDust::setEngine(CCollisionEngine *engine) {
   ready = 0;
   Engine = engine;
   Actor = Engine->addInteractor();
   Actor->setTypeId(INTERACTOR_NONCOLLIDERS);
   Actor->setCollisionMask(INTERACTOR_BLOCKS);
   Actor->getAttributes().AirStandingFriction = 0.81f;
   Actor->getAttributes().GroundStandingFriction = 0.81f;
   Actor->getAttributes().GravityMultiplier = 0.4f;
}

void CPDust::setupActor() {
   Actor->setArea(SRect2f(centerPos->X, centerPos->Y - 0.5f, 0.001f, 0.001f));
   float xvel;
   Actor->setVelocity(SVector2f(((*lookRight)?-1:1)*((float)rand()/(float)RAND_MAX*2.0f + 10.6f), 0.0f));
   Actor->addImpulse(SVector2f(xvel = ((*lookRight)?-1:1)*((float)rand()/(float)RAND_MAX*2.0f + 2.00f),
           (float)rand()/(float)RAND_MAX*1.4f + 1.8f));
   //printf("Dust XVel: %0.2f\n", xvel);
}

void CPDust::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
      if(ready) {
         translate = SVector3f((Actor->getArea().getCenter()), zFactor);
      }
      if(Counter <= 0) {
         Duration = 0;
         ready = 1;
         setupActor();
      }
   }
   else {
      Duration += timeElapsed;
      if(Actor->getVelocity().X >= 0.0001f || Actor->getVelocity().X <= -0.0001f)
         zFactor += zVelocity * timeElapsed;
      translate = SVector3f((Actor->getArea().getCenter()) + SVector2f(0.0f, 0.1f), zFactor);

      if(Duration > 1.3f) {
         Counter = (float)rand()/(float)RAND_MAX * 0.5f + 0.5f;

         zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.4f + 0.2f);
         zFactor = 0;
      }
   }
}
