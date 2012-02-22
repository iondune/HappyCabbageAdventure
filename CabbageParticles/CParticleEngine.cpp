#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration, int particleType) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   currentDuration = 0.f;
   dead = 0;

   /*
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
      //CApplication::get().getSceneManager().addSceneObject(cPtr->getRenderable());
   }
   */

   for(int i = 0; i < max; i++) {
      positionArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*1 - 0.5, (float)rand()/(float)RAND_MAX*2, 0));
      colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.8 + 0.2, (float)rand()/(float)RAND_MAX*0.8 + 0.2, (float)rand()/(float)RAND_MAX*0.8 + 0.2));
      sizeArr.push_back((float)rand()/(float)RAND_MAX*30 + 1);
   }
   myObj = new CParticleObject();
   myObj->setup(positionArr, colorArr, sizeArr, max);

   CApplication::get().getSceneManager().addSceneObject(myObj);
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
      for(int i = 0; i < numParticles; i++) {
         (*(positionArr[i])).X = cos(currentDuration*4);
         (*(positionArr[i])).Z = sin(currentDuration*4);
      }
      myObj->setPositions(positionArr);
      /*
      std::vector<CParticle*>::iterator it;
      for(it = particles.begin(); it != particles.end(); it++) {
         (* it)->updateMatrices(elapsedTime);
      }
      if(currentDuration >= totalDuration) {
         deconstruct();
      }
      */
   }
}

void CParticleEngine::deconstruct() {
   /*
   std::vector<CParticle*>::iterator it;
   for(it = particles.begin(); it != particles.end(); it++) {
      //CApplication::get().getSceneManager().removeSceneObject((* it)->getRenderable());
      //delete (* it)->getRenderable();
      delete (* it);
   }
   dead = 1;
   */
}
