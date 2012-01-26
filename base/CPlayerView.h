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


class CPlayerView {
   SVector2 CenterPosition;
   SVector2 Velocity;
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
      CPlayerView() {
         ySineValue = 0;
         yShadow = 0;
      }
      void setState(State const value);
      void setVelocity(SVector2 vel) {
         Velocity = vel;
      }
      void establishCamera();
      void setMiddle(SVector2 middle) {
         CenterPosition = middle;
      }
      void setGround(float groundYValue) {
         yShadow = groundYValue;
      }
      void draw();
      void step(float delta);
};

#endif
