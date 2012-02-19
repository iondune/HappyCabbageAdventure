#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"

class CParticleRenderable : public CMeshRenderable {
   void draw(CScene const * const scene);
};

class CParticle {
   SVector3 centerPos;
   CParticleRenderable *renderable;
   float size;
   public:
      CParticle(SVector3);
      void setupRenderable();
      CMeshRenderable * getRenderable();
};

#endif
