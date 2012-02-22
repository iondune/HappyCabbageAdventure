#ifdef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

class CParticle {
   float size;
   public:
      CMeshSceneObject *renderable;
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

      CSceneObject * getRenderable();
};

#endif
