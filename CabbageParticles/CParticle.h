#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

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
      SVector3 *centerPos;
      int* lookRight;
      float yFactor, Amplitude, Period;
      float sineValue;
      float StartFactor, AppearRate, Duration, TotalDuration, Counter;
      SVector3 RotationSpeed;

      void setLookRight(int*);
      void setCenterPos(SVector3*);
      void setAppearRate(float r);
      virtual void setupRenderable()=0;
      virtual void updateMatrices(float)=0;

      CRenderable * getRenderable();
};

#endif
