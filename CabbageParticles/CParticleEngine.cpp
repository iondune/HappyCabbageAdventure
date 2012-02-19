#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   currentDuration = 0;

   CParticle *cPtr;
   for(int i = 0; i < numParticles; i++) {
      particles.push_back(cPtr = new CParticle(&centerPos));
      cPtr->setupRenderable();
      CApplication::get().getSceneManager().addRenderable(cPtr->getRenderable());
   }
}
void CParticleEngine::setCenterPos(SVector3 cP) {
   centerPos = cP;
}
void CParticleEngine::step(float const elapsedTime) {

}
