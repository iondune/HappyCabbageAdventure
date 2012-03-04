#ifndef __CPARTICLE_ENGINE_H_____________
#define __CPARTICLE_ENGINE_H_____________

#include <vector>
#include "CParticle.h"
#include "CPCube.h"
#include "CPLeaf.h"
#include "CPFlame.h"
#include "CPDeath.h"
#include "CPLaser.h"
#include "CPLaser2.h"
#include "CPHurt.h"
#include "CPBurst.h"
#include "CPDust.h"
#include "CParticleObject.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CEngine.h"
#include "CPhysicsParticle.h"


#define LEAF_PARTICLE 0
#define CUBE_PARTICLE 1
#define FLAME_PARTICLE 2
#define DEATH_PARTICLE 3
#define LASER_CHARGING_PARTICLE 4
#define LASER_FIRING_PARTICLE 5
#define HURT_PARTICLE 6
#define BURST_PARTICLE 7
#define DUST_PARTICLE 8

class CParticleEngine {
   std::vector<CParticle*> particles;
   SVector3 centerPos;
   int numParticles;
   float totalDuration;
   float currentDuration;
   int particleType;

   std::vector<SVector3*> positionArr;
   std::vector<SVector3*> colorArr;
   std::vector<float> sizeArr;
   CParticleObject * myObj; 

   public:
      int dead, lookRight;
      //Duration in milliseconds
      CParticleEngine(SVector3 pos, int maxParticles, float duration, int);
      void setCenterPos(SVector3 cP);
      void UsePhysics(Cabbage::Collider::CEngine *engine);
      void setLookRight(int pf);
      void step(float const elapsedTime);
      void deconstruct();
};
#endif
