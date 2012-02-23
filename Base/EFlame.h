#ifndef EFLAME_H_
#define EFLAME_H_

#include "CBadGuy.h"
#include "../CabbageParticles/CParticleEngine.h"
#include "../CabbageParticles/CPFlame.h"
#include "CGameplayManager.h"

class EFlame : public CBadGuy
{

public:
   EFlame(float x, float y, float w, float h, CGameplayManager* manager);
   CParticleEngine *particleEngine;

   void update(const float TickTime);
   void doRenderable();
   void loadMesh();
   void loadActor();
};

#endif /* EKIWI_H_ */
