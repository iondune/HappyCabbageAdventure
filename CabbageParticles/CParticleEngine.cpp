#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   currentDuration = 0;
   sineValue = 0;

   CParticle *cPtr;
   for(int i = 0; i < numParticles; i++) {
      if(i % 3 == 0) {
         particles.push_back(cPtr = new CPCube());
      }
      else {
         particles.push_back(cPtr = new CPLeaf());
      }
      cPtr->setCenterPos(&centerPos);
      cPtr->setupRenderable();
      CApplication::get().getSceneManager().addRenderable(cPtr->getRenderable());
   }
}

void CParticleEngine::setCenterPos(SVector3 cP) {
   centerPos = cP;
}

void CParticleEngine::step(float const elapsedTime) {
   sineValue += 4*elapsedTime;
   std::vector<CParticle*>::iterator it;
   for(it = particles.begin(); it != particles.end(); it++) {
      (* it)->updateMatrices(sineValue);
   }
}
