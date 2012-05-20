#include "CPhysicsParticle.h"

void CPhysicsParticle::setEngine(CCollisionEngine *engine, int env) {
   Engine = engine;
   Actor = Engine->addActor();
   Actor->setTypeId(INTERACTOR_NONCOLLIDERS);
   Actor->setCollisionMask(INTERACTOR_BLOCKS);
   Actor->setArea(SRect2f(centerPos->X, centerPos->Y, 0.001f, 0.001f));

   if (env == 2) {
      Actor->getAttributes().MaxWalk *= 0.5f;
      Actor->getAttributes().WalkAccel *= 0.25f;
      Actor->getAttributes().JumpAccel *= 0.75f;
      Actor->getAttributes().JumpLength *= 2.0f;
      Actor->getAttributes().AirControl *= 8.0f;
      Actor->getAttributes().AirSpeedFactor *= 0.5f;
      }

   //Actor->setImpulse(SVector2((float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX*2.6f + 0.8f), (float)rand()/(float)RAND_MAX*3.6f + 0.6f), 0.1f);
   //Actor->Gravity = 30.0f;
}


void CPhysicsParticle::deconstruct() {
   if(Actor && Engine) {
      Engine->removeCollideable(Actor);
   }
}
