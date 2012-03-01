#include "CParticleEngine.h"

void CParticleEngine::UsePhysics(Cabbage::Collider::CEngine *engine) {
   for(int i = 0; i < numParticles; i++) {
      ((CPBurst*)particles[i])->setEngine(engine);
   }
}

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
            cPtr->setAppearRate(2);
            cPtr->useCenterPos = 1;
            break;
         case CUBE_PARTICLE:
            particles.push_back(cPtr = new CPCube());
            cPtr->setAppearRate(2);
            cPtr->useCenterPos = 1;
            break;
         case FLAME_PARTICLE:
            particles.push_back(cPtr = new CPFlame());
            cPtr->setAppearRate(2);
            cPtr->useCenterPos = 1;
            break;
         case DEATH_PARTICLE:
            particles.push_back(cPtr = new CPDeath());
            cPtr->setAppearRate(2);
            cPtr->useCenterPos = 1;
            break;
         case LASER_CHARGING_PARTICLE:
            particles.push_back(cPtr = new CPLaser());
            cPtr->setAppearRate(2);
            cPtr->useCenterPos = 1;
            break;
         case LASER_FIRING_PARTICLE:
            particles.push_back(cPtr = new CPLaser2());
            cPtr->setAppearRate(0);
            cPtr->useCenterPos = 1;
            break;
         case HURT_PARTICLE:
            particles.push_back(cPtr = new CPHurt());
            cPtr->setAppearRate(0);
            cPtr->useCenterPos = 1;
            break;
         case BURST_PARTICLE:
            particles.push_back(cPtr = new CPBurst());
            cPtr->setAppearRate(0);
            cPtr->useCenterPos = 0;
            break;
      }
      cPtr->setCenterPos(&centerPos);
      cPtr->setLookRight(&lookRight);
      cPtr->TotalDuration = duration;
      cPtr->setupRenderable();
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
               colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2f + 0.4f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.4f + 0.4f));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
            break;
         case CUBE_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3(1));
            }
            else {
               colorArr.push_back(new SVector3(temp, temp*2, temp*3));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case FLAME_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 3 == 0) {
               colorArr.push_back(new SVector3(temp*3, temp*2, temp));
            }
            else {
               colorArr.push_back(new SVector3(temp*3, temp, temp));
            }

            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case DEATH_PARTICLE:
            if(rand() % 3 == 0) {
               if(rand() % 2 == 0) {
                  colorArr.push_back(new SVector3(1.0f, 1.0f, 1.0f));
               }
               else {
                  colorArr.push_back(new SVector3(0.0f, 1.0f, 0.0f));
               }
            }
            else {
               colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.1f + 0.1f));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
            break;
         case LASER_CHARGING_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3(1));
            }
            else {
               colorArr.push_back(new SVector3(temp*3, temp*3, temp*1));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case LASER_FIRING_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3(1));
            }
            else {
               colorArr.push_back(new SVector3(temp*3, temp*3, temp*1));
            }
            sizeArr.push_back((rand() % 5 + 1)*3.0f);
            break;
         case HURT_PARTICLE:
            colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2f + 0.1f, 1.0f, (float)rand()/(float)RAND_MAX*0.4f + 0.1f));
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 65);
            break;
         case BURST_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 2 == 0)
               colorArr.push_back(new SVector3(1.0f));
            else
               colorArr.push_back(new SVector3(temp*3, temp*3, temp*1));
            //colorArr.push_back(new SVector3((float)rand()/(float)RAND_MAX*0.2f + 0.4f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.4f + 0.4f));

            sizeArr.push_back((float)rand()/(float)RAND_MAX*0 + 35);
            break;
      }
   }
   myObj = new CParticleObject();
   //Default bounding box to 0,0,0 to 1,1,1. All of our particle effects at this point (i.e. flame) are 1x1x1
   myObj->setBoundingBox(SBoundingBox3(centerPos - 0.5, centerPos + 0.5));

   const char *textureToUse;
   std::string v = "Textures/particle";

   switch(particleType) {
   case LEAF_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      break;
   case CUBE_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      /* Texture input
      std::cout << "What particle file?";
      std::cin >> buf;
      v.append(buf);
      v.append(".bmp");
      textureToUse = v.c_str();
      */
      break;
   case FLAME_PARTICLE:
      textureToUse = "Textures/particle2.bmp";
      break;
   case DEATH_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      break;
   case LASER_CHARGING_PARTICLE:
      textureToUse = "Textures/particle2.bmp";
      break;
   case LASER_FIRING_PARTICLE:
      textureToUse = "Textures/particle2.bmp";
      break;
   case HURT_PARTICLE:
      textureToUse = "Textures/particleLeaf.bmp";
      break;
   case BURST_PARTICLE:
      textureToUse = "Textures/particleStar.bmp";
      break;
   }


   myObj->setup(positionArr, colorArr, sizeArr, max, textureToUse);

   CApplication::get().getSceneManager().addSceneObject(myObj);
}

void CParticleEngine::setCenterPos(SVector3 cP) {
   centerPos = cP;
   myObj->setBoundingBox(SBoundingBox3(centerPos - 0.5, centerPos + 0.5));
   SBoundingBox3 box = myObj->getBoundingBox();
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
            if((*it)->useCenterPos == 1) {
               (*(positionArr[i]))[j] = (* it)->translate[j] + centerPos[j];
            }
            else {
               //printf("Not using center pos %0.2f %0.2f %0.2f\n", (*it)->translate.X, (*it)->translate.Y, (*it)->translate.Z);
               (*(positionArr[i]))[j] = (* it)->translate[j];
            }
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
   if(!dead) {
      std::vector<CParticle*>::iterator it;
      for(it = particles.begin(); it != particles.end(); it++) {
         //delete (* it)->getRenderable();
         delete (* it);
      }
      CApplication::get().getSceneManager().removeSceneObject(myObj);
      dead = 1;
   }
}
