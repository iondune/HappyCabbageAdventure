#ifndef ____CPDEATH_H_INCLUDE______________
#define ____CPDEATH_H_INCLUDE______________

#include "CParticle.h"

class CPDeath : public CParticle {
   void setupRenderable();
   void updateMatrices(float);
};

#endif
