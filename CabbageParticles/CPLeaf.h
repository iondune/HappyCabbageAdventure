#ifdef ____CPLEAF_H_INCLUDE______________
#define ____CPLEAF_H_INCLUDE______________

#include "CParticle.h"

class CPLeaf : public CParticle {
   void setupRenderable();
   void updateMatrices(float);
};

#endif
