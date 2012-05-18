#ifndef _______CPARTICLERISER___H__INCLUDE___
#define _______CPARTICLERISER___H__INCLUDE___

#include "CParticle.h"
#include "CCollisionEngine.h"
#include "Utils.h"

class CParticleRiser : public CParticle {
   protected:
   float globalMaxHeight;
   float globalMaxSpeed;
   float globalMinSpeed;

   float localMaxHeight;
   float yValue;
   float ySpeed;

   public:
   virtual void setupRenderable();
   virtual void updateMatrices(float);

   CParticleRiser(float, float, float);
};

#endif
