#ifdef ____CPCUBE_H_INCLUDE______________
#define ____CPCUBE_H_INCLUDE______________

#include "CParticle.h"

class CPCube : public CParticle {
   void setupRenderable();
   void updateMatrices(float);
};

#endif
