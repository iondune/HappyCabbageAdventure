#include "CParticleEngine.h"

CParticleEngine::CParticleEngine(SVector3 pos, int max, float duration, int pT) {
   centerPos = pos;
   numParticles = max;
   totalDuration = duration;
   particleType = pT;
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
         case FLAME_PARTICLE:
            particles.push_back(cPtr = new CPFlame());
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
      positionArr.push_back(new SVector3(-50));
      switch(particleType) {
         case LEAF_PARTICLE:
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3(1.0f));
            }
            else {
               colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2 + 0.4, (float)rand()/(float)RAND_MAX*0.2 + 0.8, (float)rand()/(float)RAND_MAX*0.4 + 0.4));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
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
         case FLAME_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3 + 0.3;
            if(rand() % 3 == 0) {
               colorArr.push_back(new SVector3(temp*3, temp*2, temp));
            }
            else {
               colorArr.push_back(new SVector3(temp*3, temp, temp));
            }

            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
      }
   }
   myObj = new CParticleObject();
   //Default bounding box to 0,0,0 to 1,1,1. All of our particle effects at this point (i.e. flame) are 1x1x1
   myObj->setBoundingBox(SBoundingBox3(centerPos - 0.5, centerPos + 0.5));

   char *textureToUse;

   switch(particleType) {
   case LEAF_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      break;
   case CUBE_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      break;
   case FLAME_PARTICLE:
      textureToUse = "Textures/particle2.bmp";
      break;
   }


   myObj->setup(positionArr, colorArr, sizeArr, max, textureToUse);

   CApplication::get().getSceneManager().addSceneObject(myObj);
}

void CParticleEngine::setCenterPos(SVector3 cP) {
   centerPos = cP;
   myObj->setBoundingBox(SBoundingBox3(centerPos - 0.5, centerPos + 0.5));
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
            if((* it)->Duration == -1 || (* it)->Counter > 0) {
               (*(positionArr[i]))[j] = -50;
            }
         }
      }
      myObj->setPositions(positionArr);
      if(currentDuration >= totalDuration && totalDuration >= 0) {
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
