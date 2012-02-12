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

class COverworldView {
   SVector2 CenterPosition;
   SVector2 Velocity;
   CMeshRenderable *PlayerRenderable, *renderShadow;
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
      COverworldView() {
         ySineValue = 0;
         lookRight = 1;
      }
      int lookRight;
      
      void setRenderable(CMeshRenderable *render) {
         PlayerRenderable = render;
      }
      
      void setState(State const value);
      
      void setVelocity(SVector2 vel) {
         Velocity = vel;
      }
      void establishCamera(CCamera*,int);
      void setMiddle(SVector2 middle) {
         CenterPosition = middle;
      }
      
      void draw();
      void step(float delta);
};

#endif
