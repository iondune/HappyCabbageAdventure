#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

//CParticleRenderable is just a CMeshRenderable with hierarchical transforms
class CParticleRenderable : public CRenderable {
   friend class CParticle;
   CParticleRenderable();

   CMesh * Mesh;
   std::vector<CParticleRenderable *> SubRenderables;

   CMesh * getMesh();
   void draw(CScene const * const scene);
   void setMesh(CMesh * mesh);
      
   SVector3 *centerPos;
};

class CParticle {
   SVector3  *centerPos;
   CParticleRenderable *renderable;
   float size;
   public:
      CParticle(SVector3*);
      float yFactor, Amplitude, Period;
      float StartFactor;
      SVector3 RotationSpeed;
      void setupRenderable();
      CRenderable * getRenderable();
};

#endif
