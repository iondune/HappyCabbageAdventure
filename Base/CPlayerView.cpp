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


//printf("vel %f, center %f, timer %f\n", Velocity.Y, CenterPosition.Y, zCamTimer);
   if(Velocity.Y == 0.00f  && zCamTimer <= 0.0f)
   {
     if(CenterPosition.Y > 3.0f)
     {
       float zfactor = (CenterPosition.Y - 3.0f) * 1.5f;
       zCamShift = (zfactor > 8.0f ? 8.0f :  zfactor) - zCam;
     }
     else
     {
       zCamShift = -zCam;
     }

     zCamTimer = CAM_TIMER;

   }
}
void CPlayerView::step(float delta) {
   if(time < 0)
      time = 0;
   time+=delta;
   timeChange = delta;
   if(godMode > 0)
      godMode -= delta;
   //printf("%0f\n", (time - (int)time));
   delta *= 1000.0f;
   if((int)((time - (int)time)*20.0f) % 2 == 0)
      shakeFactor = SVector3((float)rand()/(float)RAND_MAX * 0.3f - 0.15f, (float)rand()/(float)RAND_MAX * 0.3f - 0.15f, 0);
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
   Charging = 0;
   if(PlayerRenderable) {
      CApplication::get().getSceneManager().removeSceneObject(PlayerRenderable);
   }
   if(renderShadow) {
      CApplication::get().getSceneManager().removeSceneObject(renderShadow);
   }
}

void CPlayerView::addToScene() {
   Charging = 0;
   if(PlayerRenderable) {
      CApplication::get().getSceneManager().addSceneObject(PlayerRenderable);
   }
   if(renderShadow) {
      CApplication::get().getSceneManager().addSceneObject(renderShadow);
   }
}

void CPlayerView::draw() {
   int negFactor = 0;
   PlayerRenderable->setTranslation(SVector3(CenterPosition.X, CenterPosition.Y + 0.065f*sin(ySineValue), 0));
   renderShadow->setTranslation(SVector3(CenterPosition.X, yShadow + 0.01f, 0));
   if(Charging) {
      xScale = yScale = 2.0f;
      PlayerRenderable->setTranslation(SVector3(CenterPosition.X + 0.02f*sin(100.0f*ySineValue), CenterPosition.Y, 0));
      PlayerRenderable->setScale(SVector3(xScale, xScale, yScale));
      return;
   }

   //Set player's current position
   if(!((recovering > 0 || godMode > 0) && (int)(recovering *100) % 2 != 0)) {
      SVector3 rot = PlayerRenderable->getRotation();

      if(!(Velocity.Y > 0.01f || Velocity.Y < -0.01f)) {
         rot.X = 15*sin(ySineValue/2)-90.f;
         if (xScale < 2.0f) {
            xScale += .002f;
            yScale -= .002f;
         }

         else if (xScale > 2.0f) {
            xScale -= .002f;
            yScale += .002f;
         }
      }

      else if (Velocity.Y > .01f) { //When jumping/launched up
         if (yScale < 2.4f && xScale > 1.6f) {
            yScale += .0008f*Velocity.Y;
            xScale -= .0008f*Velocity.Y;
         }
      }

      else if (Velocity.Y < -0.01f) { //When falling down
         if (xScale < 2.4f && yScale > 1.6f) {
            yScale -= -1.0f*.0008f*Velocity.Y;
            xScale += -1.0f*.0008f*Velocity.Y;
         }
      }

      PlayerRenderable->setScale(SVector3(xScale, xScale, yScale));
      rot.Z = lookRight ? 80.f : 0.f;
      PlayerRenderable->setRotation(rot);
      PlayerRenderable->setVisible(true);
   }
   else
      PlayerRenderable->setVisible(false);

   //Set player's shadow
   renderShadow->setScale(SVector3((-.05f*sin(ySineValue)+1), 1,
     -0.05f*sin(ySineValue)+1));
}

void CPlayerView::establishCamera(ICamera *Camera, int angle, int shaking) {
   SVector3 camPos, camLook;
   float zfactor, yfactor;
   //yfactor = (CenterPosition.Y - 4.6f) * 0.7f;


   if(angle == 0) {
     if(zCamTimer > 0.0f &&  Velocity.Y == 0.00)
     {
       zCam += zCamShift * timeChange / CAM_TIMER;
       zCamTimer -= timeChange;
     }
     camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3f, 8 + zCam);
   }
   else if(angle == 1) {
      camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3f, 14 + zCam);
   }
   else if(angle == 2) {
      camPos = SVector3(CenterPosition.X - 4, CenterPosition.Y + 4, 1);
   }
   else {
      camPos = SVector3(CenterPosition.X + 4, CenterPosition.Y + 4, 1);
   }
   camLook = SVector3(CenterPosition.X, CenterPosition.Y, 0) - camPos;
   if(shaking == 1) {
      camPos += shakeFactor;
   }
   if(shaking >= 2) {
      camPos += shakeFactor/3.0f;
   }

   Camera->setPosition(camPos);
   Camera->setLookDirection(camLook);
   Camera->recalculateViewMatrix();
}

void CPlayerView::setShader(CShader *shad) {
   PlayerRenderable->setShader(shad);
}
