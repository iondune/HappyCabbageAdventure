#ifndef ____CPLASERCHARGED_H_INCLUDE______________
#define ____CPLASERCHARGED_H_INCLUDE______________

#include "CParticle.h"

class CPLaserCharged : public CParticle {
   float xFactor, yHeight, zFactor;
   void setupRenderable();
   void updateMatrices(float);
};

#endif
