#include "CPBurst.h"

#define RAND_POS_NEG(x) ((float)rand()/(float)RAND_MAX*2*(x) - (x))

void CPBurst::setupRenderable() {
   zVelocity = (float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX * 0.4f + 0.1f);
   zFactor = 0;
   Counter = 0;
}

void CPBurst::setEngine(Cabbage::Collider::CEngine *engine) {
   Engine = engine;
   Actor = Engine->addActor();
   Actor->CollideableLevel = INTERACTOR_NONCOLLIDERS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS;
   Actor->setArea(SRect2(centerPos->X, centerPos->Y, 0.001, 0.001));
   //Actor->setArea(SRect2(centerPos->X + RAND_POS_NEG(0.3f), centerPos->Y + RAND_POS_NEG(0.3f), 0.05, 0.05));
   Actor->setImpulse(SVector2((float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX*2.6f + 0.8f), (float)rand()/(float)RAND_MAX*3.6f + 0.6f), 0.1f);
   Actor->Gravity = 30.0f;
}

void CPBurst::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration == -1) {
      Engine->removeActor(Actor);
   }
   else {
      Duration += timeElapsed;
      zFactor += zVelocity * Actor->getVelocity().X * timeElapsed;
      translate = SVector3(Actor->getArea().getCenter() + 0.1f, zFactor);
   }
}
