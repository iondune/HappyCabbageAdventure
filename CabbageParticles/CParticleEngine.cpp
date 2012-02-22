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
      //CApplication::get().getSceneManager().addSceneObject(cPtr->getRenderable());
   }

   float temp;
   for(int i = 0; i < max; i++) {
      positionArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*1 - 0.5, (float)rand()/(float)RAND_MAX*2, 0));
      switch(particleType) {
         case LEAF_PARTICLE:
            colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2 + 0.4, (float)rand()/(float)RAND_MAX*0.2 + 0.8, (float)rand()/(float)RAND_MAX*0.4 + 0.4));
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case CUBE_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3 + 0.3;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3(1));
            }
            else {
               colorArr.push_back(new SVector3(temp, temp*2, temp*3));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
      }
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
      int i;
      std::vector<CParticle*>::iterator it;
      for(it = particles.begin(), i = 0; it != particles.end(); it++, i++) {
         (* it)->updateMatrices(elapsedTime);
         for(int j = 0; j < 3; j++) {
            (*(positionArr[i]))[j] = (* it)->translate[j] + centerPos[j];
            if((* it)->Duration == -1) {
               (*(positionArr[i]))[j] = -50;
            }
         }
      }
      myObj->setPositions(positionArr);
      if(currentDuration >= totalDuration) {
         deconstruct();
      }
   }
}

void CParticleEngine::deconstruct() {
   std::vector<CParticle*>::iterator it;
   for(it = particles.begin(); it != particles.end(); it++) {
      //delete (* it)->getRenderable();
      delete (* it);
   }
   CApplication::get().getSceneManager().removeSceneObject(myObj);
   dead = 1;
}
