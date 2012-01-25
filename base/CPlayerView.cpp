#include "CPlayerView.h"
#include <GL/glut.h>
#define PI 3.1415
void CPlayerView::setState(CPlayerView::State const value) {
   curState = value;
}
void CPlayerView::draw() {
   glPushMatrix();
   if(!(Velocity.Y > 0.01 || Velocity.Y < -0.01)) {
      if(curState == CPlayerView::State::Standing) {
         ySineValue = 0;
      }
      else if(curState == CPlayerView::State::MovingLeft) {
         ySineValue += 0.01;
      }
      else if(curState == CPlayerView::State::MovingRight) {
         ySineValue -= 0.01;
      }
   }
   else {
         ySineValue = 0;
   }
   glTranslatef(CenterPosition.X, CenterPosition.Y + 0.065*sin(ySineValue), 0);
   if(!(Velocity.Y > 0.01 || Velocity.Y < -0.01)) {
      glRotatef(15*sin(ySineValue/2), 1, 0, 0);
   }
   glutSolidSphere(0.5, 10, 10);
   //glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
   glPopMatrix();

   glDisable(GL_LIGHTING);
   //Draw Shadow
   glPushMatrix();
      glColor3f(0.2,0.2,0.2);
      glTranslatef(CenterPosition.X, yShadow + 0.01, 0);
      glScalef(-0.05*sin(ySineValue)+1, 1, -0.05*sin(ySineValue)+1);

      /* draw a solid disc from a bunch of triangles */
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
         glVertex3d(ox,0,oz);
         glVertex3d(x1,0,z1);
         glVertex3d(x,0,z);
         z1=z;
         x1=x; 
      }
      glEnd();

      glPopMatrix();
   glEnable(GL_LIGHTING);
}
void CPlayerView::establishCamera() {
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(
         CenterPosition.X, CenterPosition.Y + 1.3, 6,
         CenterPosition.X, CenterPosition.Y, 0,
         0, 1, 0);
}
