#ifndef __CPARTICLE_ENGINE_H_____________
#define __CPARTICLE_ENGINE_H_____________

#include <vector>
#include "CParticle.h"
#include "CPCube.h"
#include "CPLeaf.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"


#define LEAF_PARTICLE 0
#define CUBE_PARTICLE 1

class CParticleEngine {
   std::vector<CParticle*> particles;
   SVector3 centerPos;
   int numParticles;
   float totalDuration;
   float currentDuration;
   public:
      int dead, lookRight;
      //Duration in milliseconds
      CParticleEngine(SVector3 pos, int maxParticles, float duration, int);
      void setCenterPos(SVector3 cP);
      void setLookRight(int pf);
      void step(float const elapsedTime);
      void deconstruct();
};
#endif
