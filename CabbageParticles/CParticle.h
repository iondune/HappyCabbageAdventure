#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

//CParticleRenderable is just a CMeshRenderable with hierarchical transforms
class CParticleRenderable : public CRenderable {
   friend class CParticle;
   public:
   CParticleRenderable();

   CMesh * Mesh;
   std::vector<CParticleRenderable *> SubRenderables;

   CMesh * getMesh();
   void draw(CScene const * const scene);
   void setMesh(CMesh * mesh);
      
   SVector3 *centerPos;
};

class CParticle {
   float size;
   public:
      CParticleRenderable *renderable;
      SVector3  *centerPos;
      float yFactor, Amplitude, Period;
      float StartFactor;
      SVector3 RotationSpeed;

      void setCenterPos(SVector3*);
      virtual void setupRenderable()=0;
      virtual void updateMatrices(float)=0;

      CRenderable * getRenderable();
};

#endif
