#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   currentDuration = 0;
   sineValue = 0;

   CParticle *cPtr;
   for(int i = 0; i < numParticles; i++) {
      printf("Here in CParticleEngine's constructor\n");
      particles.push_back(cPtr = new CParticle(&centerPos));
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
      float mSineValue = sineValue + (* it)->StartFactor;
      (* it)->getRenderable()->setTranslation(
            SVector3(
               (* it)->Amplitude*cos((* it)->Period*mSineValue), 
               (* it)->yFactor, 
               (* it)->Amplitude*sin((* it)->Period*mSineValue)));
      (* it)->getRenderable()->setRotation((* it)->RotationSpeed*mSineValue);
   }
}
