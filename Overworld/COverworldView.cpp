#include "CPlayerView.h"


#ifdef _WIN32
#include <GL/glew.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#define PI 3.1415
void COverworldView::setState(CPlayerView::State const value) {
   curState = value;
}
void COverworldView::step(float delta) {
   if(!(Velocity.Y > 0.01 || Velocity.Y < -0.01)) {
      if(curState == CPlayerView::State::Standing) {
         ySineValue += 0.005f*delta;
      }
      else if(curState == CPlayerView::State::MovingLeft) {
         ySineValue += 0.01f*delta;
         lookRight = 0;
      }
      else if(curState == CPlayerView::State::MovingRight) {
         ySineValue -= 0.01f*delta;
         lookRight = 1;
      }
   }
   else {
         ySineValue = 0;
   }
}

void COverworldView::draw() {
   int negFactor = 0;

   //Set player's current position
   PlayerRenderable->setTranslation(SVector3(CenterPosition.X, CenterPosition.Y + 0.065f*sin(ySineValue), 0));
   SVector3 rot = PlayerRenderable->getRotation();
   if(!(Velocity.Y > 0.01f || Velocity.Y < -0.01f)) {
      rot.X = 15*sin(ySineValue/2)-90.f;
   }
   rot.Z = lookRight ? 80.f : 0.f;
   PlayerRenderable->setRotation(rot);
   PlayerRenderable->setVisible(true);
}

void COverworldView::establishCamera(CCamera *Camera, int angle) {
   SVector3 camPos, camLook;

   camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3f, 6);
   
   camLook = SVector3(CenterPosition.X, CenterPosition.Y, 0) - camPos;
   Camera->setPosition(camPos);
   Camera->setLookDirection(camLook);
   Camera->recalculateViewMatrix();
}
