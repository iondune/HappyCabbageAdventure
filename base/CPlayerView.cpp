#include "CPlayerView.h"


#ifdef _WIN32
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif


#define PI 3.1415
void CPlayerView::setState(CPlayerView::State const value) {
   curState = value;
}
void CPlayerView::step(float delta) {
   if(!(Velocity.Y > 0.01 || Velocity.Y < -0.01)) {
      if(curState == CPlayerView::State::Standing) {
         ySineValue = 0;
      }
      else if(curState == CPlayerView::State::MovingLeft) {
         ySineValue += 0.01*delta;
      }
      else if(curState == CPlayerView::State::MovingRight) {
         ySineValue -= 0.01*delta;
      }
   }
   else {
         ySineValue = 0;
   }
}

void CPlayerView::draw() {

   //Set player's current position
   if(!(recovering > 0 && (int)(recovering *100) % 2 != 0)) {
      setMaterial(LIGHT_GREEN_MATTE);
      PlayerRenderable->setTranslation(SVector3(CenterPosition.X, CenterPosition.Y + 0.065*sin(ySineValue), 0));
      if(!(Velocity.Y > 0.01 || Velocity.Y < -0.01)) {
         PlayerRenderable->setRotation(SVector3(15*sin(ySineValue/2)-90, 0, 0));
      }
   }

   //Set player's shadow
   renderShadow->setTranslation(SVector3(CenterPosition.X, yShadow + 0.01, 0));
   renderShadow->setScale(SVector3(-.05*sin(ySineValue)+1, 1,
     -0.05*sin(ySineValue)+1));



   //Draw Shadow
   /*glPushMatrix();
      glTranslatef(CenterPosition.X, yShadow + 0.01, 0);
      glScalef(-0.05*sin(ySineValue)+1, 1, -0.05*sin(ySineValue)+1);

      // draw a solid disc from a bunch of triangles //
      float z1,x1,x,z,ox=0,oz=0,angle,radius=0.5;
      z1=oz;
      x1=ox;
      glBegin(GL_TRIANGLES);  
      int i;
      for(i=0;i<=360;i++)
      {
         angle=(float)(((double)i)/57.29577957795135);   
         x=ox+(radius*(float)sin((double)angle));
         z=oz+(radius*(float)cos((double)angle));      
         glVertex3f(ox,0,oz);
         glVertex3f(x1,0,z1);
         glVertex3f(x,0,z);
         z1=z;
         x1=x; 
      }
      glEnd();
   glPopMatrix();*/
}

void CPlayerView::establishCamera(CCamera *Camera, int angle) {
   SVector3 camPos, camLook;

   if(angle == 0) {
      camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3, 6);
   }
   else if(angle == 1) {
      camPos = SVector3(CenterPosition.X, CenterPosition.Y + 1.3, -6);
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
