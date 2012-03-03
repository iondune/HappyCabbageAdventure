#include "CPhysicsParticle.h"

void CPhysicsParticle::setEngine(Cabbage::Collider::CEngine *engine) {
   Engine = engine;
   Actor = Engine->addActor();
   Actor->CollideableLevel = INTERACTOR_NONCOLLIDERS;
   Actor->CanCollideWith = INTERACTOR_BLOCKS;
   Actor->setArea(SRect2(centerPos->X, centerPos->Y, 0.001, 0.001));
   //Actor->setImpulse(SVector2((float)(rand() % 2 * 2 - 1)*((float)rand()/(float)RAND_MAX*2.6f + 0.8f), (float)rand()/(float)RAND_MAX*3.6f + 0.6f), 0.1f);
   //Actor->Gravity = 30.0f;
}

