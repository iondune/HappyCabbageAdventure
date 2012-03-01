#include "CPBurst.h"

void CPBurst::setupRenderable() {
   zVelocity = (float)rand()/(float)RAND_MAX * 0.6f - 0.3f;
   zFactor = 0;
   Counter = 0;
}

void CPBurst::setEngine(Cabbage::Collider::CEngine *engine) {
   Engine = engine;
   Actor = Engine->addActor();
   Actor->CollideableLevel = INTERACTOR_BLOCKS;
}

void CPBurst::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration == -1) {
      Engine->removeActor(Actor);
   }
   else {
      sineValue += 4*timeElapsed;
      Duration += timeElapsed;
      zFactor += zVelocity * timeElapsed;
      translate = SVector3(Actor->getArea(), zFactor);
   }
}
