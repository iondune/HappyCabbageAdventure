#include "CMainMenuState.h"

CMainMenuState::CMainMenuState() {}

void CMainMenuState::setupTextures()
{
   woodTexture = new CTexture(CImageLoader::loadImage("wood011.bmp"));
   skyTexture = new CTexture(CImageLoader::loadImage("sky.bmp"));
}

void CMainMenuState::drawSky(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, skyTexture->getTextureHandle());

   glBegin(GL_QUADS);
   if(!backwards) {
      glTexCoord2f(0, 1);
      glVertex3f(-25, 22, -2.5);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -1, -2.5);
      glTexCoord2f(1, 0);
      glVertex3f(25, -1, -2.5);
      glTexCoord2f(1, 1);
      glVertex3f(25, 22, -2.5);
   }
   else {
      glTexCoord2f(0, 1);
      glVertex3f(-2, 2, 0.01);
      glTexCoord2f(0, 0);
      glVertex3f(-2, -2, 0.01);
      glTexCoord2f(1, 0);
      glVertex3f(2, -2, 0.01);
      glTexCoord2f(1, 1);
      glVertex3f(2, 2, 0.01);
   }
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void CMainMenuState::drawButton()
{
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, woodTexture->getTextureHandle());

   glScalef(0.65, 0.25, 1.0);

   glBegin( GL_QUADS );
   //Draw square
   glTexCoord2f(0,0);
   glVertex3f( 0,0,0 );
   glTexCoord2f(1,0);
   glVertex3f( 1, 0,0 );
   glTexCoord2f(1,1);
   glVertex3f( 1, 1, 0 );
   glTexCoord2f(0,1);
   glVertex3f( 0,1, 0 );
   //End quad
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void CMainMenuState::begin()
{
   printf("this begins it\n");
   setupTextures();
}
void CMainMenuState::end()
{
   printf("this ends it\n");
}

void CMainMenuState::OnRenderStart(float const Elapsed)
{
   //Start quad
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_LIGHTING);

   glPushMatrix();

   drawSky(true);

   glTranslatef(-0.3,0,0);
   drawButton();
   glTranslatef(0,-0.35, 0);
   drawButton();
   glTranslatef(0,-0.35, 0);
   drawButton();

   glPopMatrix();

   glEnable(GL_LIGHTING);

   glLoadIdentity();
   SDL_GL_SwapBuffers();
}
void CMainMenuState::OnRenderEnd(float const Elapsed)
{

}


void CMainMenuState::OnMouseEvent(SMouseEvent const & Event)
{
   if(Event.Type.Value == SMouseEvent::EType::Move){
      //printf("this is mouse move: Location x %d y %d\n",Event.Location.X, Event.Location.Y);
   }
   else if(Event.Type.Value == SMouseEvent::EType::Click){
      //printf("this is mouse click: Location x %d y %d ",Event.Location.X, Event.Location.Y);
      //printf(" Pressed %d ", Event.Pressed);

      switch (Event.Button.Value){

      case SMouseEvent::EButton::Left:
         //printf(" Left\n");

         if(!Event.Pressed && Event.Location.X < 540 && Event.Location.X > 280)
         {
            if(Event.Location.Y >225 && Event.Location.Y < 300)
            {
               CApplication::get().getStateManager().setState(& CGameState::get());
               printf("top button hit!!\n");
            } 
            else if(Event.Location.Y >330 && Event.Location.Y < 400)
            {
               printf("mid button hit!!\n");
            } 
            else if(Event.Location.Y >435  && Event.Location.Y < 510)
            {
               printf("bot button hit!!\n");
            }
         }
         break;
      case SMouseEvent::EButton::Right:
         //printf(" Right\n", Event.Pressed);
         break;
      case SMouseEvent::EButton::Middle:
         //printf(" Middle\n", Event.Pressed);
         break;
      }


   }
}
void CMainMenuState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   printf("this is key\n");
}
