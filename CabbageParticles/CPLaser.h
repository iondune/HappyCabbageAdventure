#ifndef ____CPLASER_H_INCLUDE______________
#define ____CPLASER_H_INCLUDE______________

#include "CParticle.h"

class CPLaser : public CParticle {
   void setupRenderable();
   void updateMatrices(float);
};

#endif
