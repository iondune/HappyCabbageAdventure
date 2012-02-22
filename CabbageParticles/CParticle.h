#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

class CParticle {
   float size;
   public:
      SVector3 *centerPos;
      int* lookRight;
      float yFactor, Amplitude, Period;
      float sineValue;
      float StartFactor, AppearRate, Duration, TotalDuration, Counter;
      float Size;
      SVector3 RotationSpeed;

      SVector3 translate;

      void setLookRight(int*);
      void setCenterPos(SVector3*);
      void setAppearRate(float r);
      virtual void setupRenderable()=0;
      virtual void updateMatrices(float)=0;
};

#endif
