#ifndef ____CPBURST_H_INCLUDE______________
#define ____CPBURST_H_INCLUDE______________

#include "CParticle.h"
#include "CEngine.h"

class CGameState;
class CPBurst : public CParticle {
   float zFactor, zVelocity;
   float Size;

   Cabbage::Collider::CActor *Actor;

   void setupRenderable();
   void updateMatrices(float);
};

#endif
