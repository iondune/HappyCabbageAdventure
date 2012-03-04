#include "CPDust.h"

#define RAND_POS_NEG(x) ((float)rand()/(float)RAND_MAX*2*(x) - (x))

void CPDust::setupRenderable() {
   ready = 0;
   zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.2f + 0.1f);
   zFactor = 0;
}

void CPDust::setEngine(Cabbage::Collider::CEngine *engine) {
   ready = 0;
   Engine = engine;
   Actor = Engine->addActor();
   Engine->removeActor(Actor);
}

void CPDust::setupActor() {
   Engine->removeActor(Actor);
   Actor = Engine->addActor();
   Actor->CollideableLevel = INTERACTOR_NONCOLLIDERS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS;
   //Actor->setArea(SRect2(0.0f, 0.0f, 0.001, 0.001));
   Actor->setArea(SRect2(centerPos->X, centerPos->Y - 0.5f, 0.001f, 0.001f));
   //Actor->setArea(SRect2(centerPos->X + RAND_POS_NEG(0.3f), centerPos->Y + RAND_POS_NEG(0.3f), 0.05, 0.05));
   Actor->setImpulse(SVector2(((*lookRight)?1:-1)*((float)rand()/(float)RAND_MAX*3.6f + 0.9f), (float)rand()/(float)RAND_MAX*0.3f + 0.9f), 0.1f);
   Actor->Gravity = 40.0f;
}

void CPDust::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
      if(ready) {
         translate = SVector3((Actor->getArea().getCenter()), zFactor);
      }
      if(Counter <= 0) {
         Duration = 0;
         ready = 1;
         setupActor();
      }
   }
   /*
   else if(Duration == -1) {
      Engine->removeActor(Actor);
   }
   */
   else {
      Duration += timeElapsed;
      zFactor += zVelocity * Actor->getVelocity().X * timeElapsed;
      translate = SVector3((Actor->getArea().getCenter()) + SVector2(0.0f, 0.1f), zFactor);

      if(Duration > 0.8f) {
         Counter = (float)rand()/(float)RAND_MAX * 0.5f + 0.5f;

         zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.2f + 0.1f);
         zFactor = 0;
      }
   }
}
