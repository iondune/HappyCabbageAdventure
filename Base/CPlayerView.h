#ifndef __CPLAYER_VIEW_H_
#define __CPLAYER_VIEW_H_ 1

#include "../CabbageCore/SVector2.h"
#include "Materials.h"
#include <math.h>

#ifdef _WIN32
#include <GL/glew.h>
#endif

#ifdef __unix__
#include <GL/gl.h>
#endif

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

class CPlayerView {
   SVector2 CenterPosition;
   SVector2 Velocity;
   CMeshSceneObject *PlayerRenderable, *renderShadow;
   float recovering;
   private:
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
      float yShadow;
      float xScale;
      float yScale;
      float time;
      SVector3 shakeFactor;
      int Charging;
      CPlayerView() {
         ySineValue = 0;
         yShadow = 0;
         lookRight = 1;
         yScale = xScale = 2.0f;
         time = 0;
         shakeFactor = SVector3(0.0f);
      }
      int lookRight;

      int getLookRight();
      void setRenderable(CMeshSceneObject *render, CMeshSceneObject *shadowRender) {
         PlayerRenderable = render;
         renderShadow = shadowRender;
      }
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
      void setGround(float groundYValue) {
         yShadow = groundYValue;
      }
      void setShader(CShader * shader);
      void draw();
      void step(float delta);
};

#endif
