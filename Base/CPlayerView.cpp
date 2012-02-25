#include "CPlayerView.h"


#ifdef _WIN32
#include <GL/glew.h>
#endif

#ifdef __unix__
#include <GL/gl.h>
#endif


#define PI 3.1415
void CPlayerView::setState(CPlayerView::State const value) {
   curState = value;
}
void CPlayerView::step(float delta) {
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

int CPlayerView::getLookRight() {
   return lookRight;
}

void CPlayerView::removeFromScene() {
   if(PlayerRenderable) {
      CApplication::get().getSceneManager().removeSceneObject(PlayerRenderable);
   }
   if(renderShadow) {
      CApplication::get().getSceneManager().removeSceneObject(renderShadow);
   }
}

void CPlayerView::addToScene() {
   if(PlayerRenderable) {
      CApplication::get().getSceneManager().addSceneObject(PlayerRenderable);
   }
   if(renderShadow) {
      CApplication::get().getSceneManager().addSceneObject(renderShadow);
   }
}

void CPlayerView::draw() {
   int negFactor = 0;

   //Set player's current position
   if(!(recovering > 0 && (int)(recovering *100) % 2 != 0)) {
      PlayerRenderable->setTranslation(SVector3(CenterPosition.X, CenterPosition.Y + 0.065f*sin(ySineValue), 0));
      SVector3 rot = PlayerRenderable->getRotation();
      if(!(Velocity.Y > 0.01f || Velocity.Y < -0.01f)) {
         rot.X = 15*sin(ySineValue/2)-90.f;
      }
      rot.Z = lookRight ? 80.f : 0.f;
      PlayerRenderable->setRotation(rot);
      PlayerRenderable->setVisible(true);
   }
   else
      PlayerRenderable->setVisible(false);

   //Set player's shadow
   renderShadow->setTranslation(SVector3(CenterPosition.X, yShadow + 0.01f, 0));
   renderShadow->setScale(SVector3((-.05f*sin(ySineValue)+1), 1,
     -0.05f*sin(ySineValue)+1));
}

void CPlayerView::establishCamera(ICamera *Camera, int angle) {
   SVector3 camPos, camLook;

   if(angle == 0) {
      camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3f, 6);
   }
   else if(angle == 1) {
      camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3f, 10);
   }
   else if(angle == 2) {
      camPos = SVector3(CenterPosition.X - 4, CenterPosition.Y + 4, 1);
   }
   else {
      camPos = SVector3(CenterPosition.X + 4, CenterPosition.Y + 4, 1);
   }
   camLook = SVector3(CenterPosition.X, CenterPosition.Y, 0) - camPos;
   Camera->setPosition(camPos);
   Camera->setLookDirection(camLook);
   Camera->recalculateViewMatrix();
}
