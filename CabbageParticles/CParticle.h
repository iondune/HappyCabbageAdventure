#ifndef __CPARTICLE__H_____
#define __CPARTICLE__H_____

#include "CabbageCore.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

class CParticle {
   float size;
   public:
      SVector3f *centerPos;
      int* lookRight;
      float StartFactor, AppearRate, Duration, TotalDuration, Counter;

      //SVector3f RotationSpeed;
      SVector3f translate;

      int useCenterPos;
      void setLookRight(int*);

      virtual void deconstruct();
      virtual void setCenterPos(SVector3*);
      void setAppearRate(float r);
      virtual void setupRenderable()=0;
      virtual void updateMatrices(float)=0;
};
#endif
