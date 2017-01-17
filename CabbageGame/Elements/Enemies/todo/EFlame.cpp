#include "EFlame.h"

EFlame::EFlame(float x, float y, float w, float h, CGameplayManager* manager) {
   this->x = x;
   this->y = y;
   this->w = w;
   this->h = h;
   Manager = manager;

   loadMesh();
   loadActor();
}

//Loads and moves the mesh
void EFlame::loadMesh() {
   particleEngine = new CParticleEngine(SVector3f(x, y, 0), 100, -1, FLAME_PARTICLE);
}

//Adds actor to engine and preps engine
void EFlame::loadActor() {
   Actor = Manager->getEngine()->addActor();

   //Makes them immune to gravity
   Actor->setControlFall(false);
   Actor->setGravity(0.0f);

   Actor->setArea(SRect2f(SVector2f(x, y), SVector2f(w, h)));

   //Set actor attributes
   Actor->getAttributes().MaxWalk = 3.0f;
   Actor->getAttributes().WalkAccel = 20.0f;

   Actor->getAttributes().AirControl = 1.0f;
   Actor->getAttributes().AirSpeedFactor = 1.0f;
}

void EFlame::doRenderable() {
}


//Updates AI's decision per frame
void EFlame::update(float const TickTime) {
   particleEngine->step(TickTime);
}
