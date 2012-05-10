#include "CParticleEngine.h"

void CParticleEngine::UsePhysics(CCollisionEngine *engine) {
   for(int i = 0; i < numParticles; i++) {
      ((CPhysicsParticle*)particles[i])->setEngine(engine);
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
         case LASER_CHARGED_PARTICLE:
            particles.push_back(cPtr = new CPLaserCharged());
            cPtr->setAppearRate(2.3f);
            cPtr->useCenterPos = 1;
            break;
         case LASER_CHARGING_PARTICLE:
            particles.push_back(cPtr = new CPLaser());
            cPtr->setAppearRate(0.8f);
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
         case DUST_PARTICLE:
            particles.push_back(cPtr = new CPDust());
            cPtr->setAppearRate(2.0f);
            cPtr->useCenterPos = 0;
            break;
         case STAR_PARTICLE:
            particles.push_back(cPtr = new CPStar());
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
      positionArr.push_back(new SVector3f(-50));
      switch(particleType) {
         case LEAF_PARTICLE:
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3f(1.0f));
            }
            else {
               colorArr.push_back(new SVector3f((float)rand()/(float)RAND_MAX*0.2f + 0.4f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.4f + 0.4f));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
            break;
         case CUBE_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3f(1));
            }
            else {
               colorArr.push_back(new SVector3f(temp, temp*2, temp*3));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case FLAME_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 3 == 0) {
               colorArr.push_back(new SVector3f(temp*3, temp*2, temp));
            }
            else {
               colorArr.push_back(new SVector3f(temp*3, temp, temp));
            }

            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
            break;
         case DEATH_PARTICLE:
            if(rand() % 3 == 0) {
               if(rand() % 2 == 0) {
                  colorArr.push_back(new SVector3f(1.0f, 1.0f, 1.0f));
               }
               else {
                  colorArr.push_back(new SVector3f(0.0f, 1.0f, 0.0f));
               }
            }
            else {
               colorArr.push_back(new SVector3f((float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.1f + 0.1f));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 15);
            break;
         case LASER_CHARGED_PARTICLE:
         case LASER_CHARGING_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3f(1));
            }
            else {
               colorArr.push_back(new SVector3f(temp*3, temp*3, temp*1));
            }
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 10);
            break;
         case LASER_FIRING_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 4 == 0) {
               colorArr.push_back(new SVector3f(1));
            }
            else {
               colorArr.push_back(new SVector3f(temp*3, temp*3, temp*1));
            }
            sizeArr.push_back((rand() % 5 + 1)*3.0f);
            break;
         case HURT_PARTICLE:
            colorArr.push_back(new SVector3f((float)rand()/(float)RAND_MAX*0.2f + 0.1f, 1.0f, (float)rand()/(float)RAND_MAX*0.4f + 0.1f));
            sizeArr.push_back((float)rand()/(float)RAND_MAX*5 + 65);
            break;
         case BURST_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 2 == 0)
               colorArr.push_back(new SVector3f(1.0f));
            else
               colorArr.push_back(new SVector3f(temp*3, temp*3, temp*1));
            //colorArr.push_back(new SVector3f((float)rand()/(float)RAND_MAX*0.2f + 0.4f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.4f + 0.4f));

            sizeArr.push_back((float)rand()/(float)RAND_MAX*0 + 35);
            break;
         case DUST_PARTICLE:
            temp = (float)rand()/(float)RAND_MAX*0.3f + 0.3f;
            if(rand() % 6 == 0)
               colorArr.push_back(new SVector3f(1.0f));
            else
               colorArr.push_back(new SVector3f(temp*3, temp*3, temp*1));
            //colorArr.push_back(new SVector3f((float)rand()/(float)RAND_MAX*0.2f + 0.4f, (float)rand()/(float)RAND_MAX*0.2f + 0.8f, (float)rand()/(float)RAND_MAX*0.4f + 0.4f));

            sizeArr.push_back((float)rand()/(float)RAND_MAX*10 + 10);
            break;
         case STAR_PARTICLE:
            temp = 0.2f;
            colorArr.push_back(new SVector3f(1.0f - (float)rand()/(float)RAND_MAX*temp, 1.0f - (float)rand()/(float)RAND_MAX*temp, 1.0f - (float)rand()/(float)RAND_MAX*temp));
            sizeArr.push_back((float)rand()/(float)RAND_MAX*60 + 30);
            break;
      }
   }
   myObj = new CParticleObject();
   //Default bounding box to 0,0,0 to 1,1,1. All of our particle effects at this point (i.e. flame) are 1x1x1
   myObj->setBoundingBox(SBoundingBox3(centerPos - 0.7, centerPos + 0.7));

   const char *textureToUse;
   std::string v = "Base/particle";

   switch(particleType) {
   case LEAF_PARTICLE:
      textureToUse = "Base/particleStar.bmp";
      break;
   case CUBE_PARTICLE:
      textureToUse = "Base/particleStar.bmp";
      /* Texture input
      std::cout << "What particle file?";
      std::cin >> buf;
      v.append(buf);
      v.append(".bmp");
      textureToUse = v.c_str();
      */
      break;
   case FLAME_PARTICLE:
      textureToUse = "Base/particleTriangle2.bmp";
      break;
   case DEATH_PARTICLE:
      textureToUse = "Base/particleStar.bmp";
      break;
   case LASER_CHARGED_PARTICLE:
   case LASER_CHARGING_PARTICLE:
      textureToUse = "Base/particle2.bmp";
      break;
   case LASER_FIRING_PARTICLE:
      textureToUse = "Base/particle2.bmp";
      break;
   case HURT_PARTICLE:
      textureToUse = "Base/particleLeaf.bmp";
      break;
   case BURST_PARTICLE:
      textureToUse = "Base/particleStar.bmp";
      break;
   case DUST_PARTICLE:
      textureToUse = "Base/particle2.bmp";
      break;
   case STAR_PARTICLE:
      textureToUse = "Base/particleStar.bmp";
      myObj->setAlwaysRender();
      myObj->setSizeFactor(38.0f);
      break;
   }


   myObj->setup(positionArr, colorArr, sizeArr, max, textureToUse);

   CApplication::get().getSceneManager().addPostOpaqueSceneObject(myObj);
   /*
    * Doesn't work when I turn off updating for immobile objects.
   // This makes flame particles fixed in place. If you're going to add a new flame particle after the first frame has been drawn, there might be problems. Talk to Alden.
   if(particleType != FLAME_PARTICLE) {
      CApplication::get().getSceneManager().addSceneObject(myObj);
   }
   else {
      CApplication::get().getSceneManager().addImmobileSceneObject(myObj, THIS_OBJECT_WILL_NEVER_MOVE_AND_ITS_BOUNDING_BOX_IS_CORRECT);
   }
   */
}

void CParticleEngine::setCenterPos(SVector3 cP) {
   if(!dead) {
      centerPos = cP;
      myObj->setBoundingBox(SBoundingBox3(centerPos - 0.5, centerPos + 0.5));
      SBoundingBox3 box = myObj->getBoundingBox();
   }
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
         (* it)->deconstruct();
         delete (* it);
      }
      CApplication::get().getSceneManager().removeSceneObject(myObj);
      dead = 1;
   }
}

void CParticleEngine::setVisible(bool visible) {
   myObj->setVisible(visible);
}

bool CParticleEngine::isVisible() {
   return myObj->isVisible();
}
