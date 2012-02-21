#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration, int particleType) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   currentDuration = 0.f;
   dead = 0;

   CParticle *cPtr;
   for(int i = 0; i < numParticles; i++) {
      cPtr = NULL;
      switch(particleType) {
         case LEAF_PARTICLE:
            particles.push_back(cPtr = new CPLeaf());
            break;
         case CUBE_PARTICLE:
            particles.push_back(cPtr = new CPCube());
            break;
      }
      cPtr->setCenterPos(&centerPos);
      cPtr->setLookRight(&lookRight);
      cPtr->setAppearRate(2);
      cPtr->TotalDuration = duration;
      cPtr->setupRenderable();
      CApplication::get().getSceneManager().addRenderable(cPtr->getRenderable());
   }
}

void CParticleEngine::setCenterPos(SVector3 cP) {
   centerPos = cP;
}

void CParticleEngine::setLookRight(int pf) {
   lookRight = pf; 
}

void CParticleEngine::step(float const elapsedTime) {
   if(!dead) {
      currentDuration += elapsedTime;
      std::vector<CParticle*>::iterator it;
      for(it = particles.begin(); it != particles.end(); it++) {
         (* it)->updateMatrices(elapsedTime);
      }
      if(currentDuration >= totalDuration) {
         deconstruct();
      }
   }
}

void CParticleEngine::deconstruct() {
   std::vector<CParticle*>::iterator it;
   for(it = particles.begin(); it != particles.end(); it++) {
      CApplication::get().getSceneManager().removeRenderable((* it)->getRenderable());
      delete (* it)->getRenderable();
      delete (* it);
   }
   dead = 1;
}
