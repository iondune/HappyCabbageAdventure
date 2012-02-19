#ifndef __CPARTICLE_ENGINE_H_____________
#define __CPARTICLE_ENGINE_H_____________

#include <vector>
#include "CParticle.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

class CParticleEngine {
   std::vector<CParticle*> particles;
   SVector3 centerPos;
   int numParticles;
   float totalDuration;
   float currentDuration;
   public:
      //Duration in seconds
      CParticleEngine(SVector3 pos, int maxParticles, float duration);
      void setCenterPos(SVector3 cP);
      void step(float const elapsedTime);
};
#endif
