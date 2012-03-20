#ifndef __CPLAYER_VIEW_H_
#define __CPLAYER_VIEW_H_ 1

#include "../CabbageCore/SVector2.h"
#include <math.h>

#ifdef _WIN32
#include <GL/glew.h>
#endif

#ifdef __unix__
#include <GL/gl.h>
#endif

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#define CAM_TIMER 0.2f

class CPlayerView {
   SVector2 Size;
   SVector2 CenterPosition;
   SVector2 Velocity;
   CMeshSceneObject *PlayerRenderable, *renderLeftShadow, *renderRightShadow;
   float recovering;
   private:
    float zCam,zCamShift,zCamTimer;
   public:
      class State {
         public:
            enum SState {
               Standing,
               MovingLeft,
               MovingRight,
            };
         private:
            SState Value;
         public:
            State() : Value(Standing) {}
            State(SState const value) : Value(value) {}
            bool const operator == (SState const value) {
               return Value == value;
            }
      };
      State curState;
      float ySineValue;
      float yLeftShadow, yRightShadow;
      float xScale;
      float yScale;
      float time,timeChange;
      SVector3 shakeFactor;
      int Charging;

	  SUniform<float> LeftShadowStart, LeftShadowCutoff, RightShadowStart, RightShadowCutoff;
	  float LeftShadowStartValue, LeftShadowCutoffValue, RightShadowStartValue, RightShadowCutoffValue;

      CPlayerView() 
		  : LeftShadowStart(LeftShadowStartValue), LeftShadowCutoff(LeftShadowCutoffValue), 
		  RightShadowCutoff(RightShadowCutoffValue), RightShadowStart(RightShadowStartValue)
	  {
         ySineValue = 0;
         yRightShadow = yLeftShadow = 0;
         lookRight = 1;
         yScale = xScale = 2.0f;
         time = 0;
         shakeFactor = SVector3(0.0f);
         zCam = 0.0f;
         zCamTimer = 0.0f;
      }
      int lookRight;

      int getLookRight();
      void setRenderable(CMeshSceneObject *render, CMeshSceneObject *shadowLeftRender, CMeshSceneObject *shadowRightRender) {
         PlayerRenderable = render;
         renderLeftShadow = shadowLeftRender;
         renderRightShadow = shadowRightRender;
      }
      float godMode;
      void setGodMode(float i) {godMode = i;}
      void removeFromScene();
      void addToScene();
      void setRecovering(float recovVal) {
         recovering = recovVal;
      }
      void setState(State const value);
      void setVelocity(SVector2 vel) {
         Velocity = vel;
      }
      void establishCamera(ICamera*,int,int);
      void setMiddle(SVector2 middle) {
         CenterPosition = middle;
      }
      void setSize(SVector2 size) {
         Size = size;
      }
      float cutOffPoint;
      void setCutoffPoint(SRect2 left, SRect2 right) {
         renderLeftShadow->setVisible(left.Size.X != 0.0f);
         renderRightShadow->setVisible(right.Size.X != 0.0f);
         bool equal = false;

         if(left.otherCorner().Y > right.otherCorner().Y) {
            cutOffPoint = left.Position.X + left.Size.X;
         }
         else if(left.otherCorner().Y < right.otherCorner().Y) {
            cutOffPoint = right.Position.X;
         }
         else {
            equal = true;
            cutOffPoint = 0.0f;
         }
         float oldCutOff = cutOffPoint;
         if(!equal) {
            cutOffPoint = (cutOffPoint - (CenterPosition.X - Size.X/2.f))/Size.X;
         }
      }
      void setLeftGround(float groundYValue) {
         yLeftShadow = groundYValue;
      }
      void setRightGround(float groundYValue) {
         yRightShadow = groundYValue;
      }
      void setShader(CShader * shader, CShader * dShader);
      void draw();
      void step(float delta);
};

#endif
