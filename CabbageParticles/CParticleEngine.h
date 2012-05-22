#ifndef __CPARTICLE_ENGINE_H_____________
#define __CPARTICLE_ENGINE_H_____________

#include <vector>
#include "CParticle.h"
#include "CParticleObject.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CCollisionEngine.h"
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
#define STAR_PARTICLE 9
#define LASER_CHARGED_PARTICLE 10
#define WIGGLE_PARTICLE 11
#define BUBBLE_PARTICLE 12
#define UNDERWATER_STAR_PARTICLE 13

class CParticleEngine {
   std::vector<CParticle*> particles;
   SVector3f centerPos;
   int numParticles;
   float totalDuration;
   float currentDuration;
   int particleType;

   std::vector<SVector3f*> positionArr;
   std::vector<SVector3f*> colorArr;
   std::vector<float> sizeArr;
   CParticleObject * myObj; 

   public:
      int dead, lookRight;
      //Duration in milliseconds
      CParticleEngine(SVector3f pos, int maxParticles, float duration, int, bool night = false);
      void setCenterPos(SVector3f cP);
      void UsePhysics(CCollisionEngine *engine, int env);
      void setLookRight(int pf);
      void step(float const elapsedTime);
      void deconstruct();
      void setVisible(bool visible);
      bool isVisible();
      ISceneObject * getSceneObject();
};
#endif
