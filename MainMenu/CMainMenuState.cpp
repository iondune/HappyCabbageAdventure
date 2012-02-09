#include "CMainMenuState.h"

CMainMenuState::CMainMenuState() 
: Application (CApplication::get())
{}

void CMainMenuState::setupTextures()
{
   woodTexture = new CTexture(CImageLoader::loadImage("wood011.bmp"));
   skyTexture = new CTexture(CImageLoader::loadImage("sky.bmp"));
}

void CMainMenuState::setupMeshes()
{
  CShader *Flat;
  Flat = CShaderLoader::loadShader("Diffuse");
  logoMesh = CMeshLoader::load3dsMesh("../base/Models/HappyLogo1.3ds");
  if (logoMesh) 
  {
    logoMesh->resizeMesh(SVector3(1.0));
    logoMesh->centerMeshByExtents(SVector3(0));
    logoMesh->calculateNormalsPerFace();
  }
  else 
  {
    fprintf(stderr, "Failed to load the logo mesh\n");
  }

  renderLogo = new CMeshRenderable();
  renderLogo->setMesh(logoMesh);
  renderLogo->getMaterial().Shader = Flat;
  renderLogo->setTranslation(SVector3(0.00, 0.1, 0.3));
  //renderLogo->setTranslation(SVector3(0.00, 0.1, 0.3));
  renderLogo->setScale(SVector3(0.4));
  renderLogo->setRotation(SVector3(75, 180, 0));
}

void CMainMenuState::drawSky(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, skyTexture->getTextureHandle());

   glTranslatef(0,0,1.01f);
   
   glBegin(GL_QUADS);
   //if(!backwards) {
      glTexCoord2f(0, 1);
      glVertex3f(-20, 22, -2.0f);
      glTexCoord2f(0, 0);
      glVertex3f(-20, -2, -2.0f);
      glTexCoord2f(1, 0);
      glVertex3f(20, -2, -2.0f);
      glTexCoord2f(1, 1);
      glVertex3f(20, 22, -2.0f);
   //}
  /* else {
      glTexCoord2f(0, 1);
      glVertex3f(-2, 2, 0.0f);
      glTexCoord2f(0, 0);
      glVertex3f(-2, -2, 0.0f);
      glTexCoord2f(1, 0);
      glVertex3f(2, -2, 0.0f);
      glTexCoord2f(1, 1);
      glVertex3f(2, 2, 0.0f);
   }*/
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void CMainMenuState::drawButton()
{
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, woodTexture->getTextureHandle());

   glScalef(0.65f, 0.25f, 1.f);

   glBegin( GL_QUADS );
   //Draw square
   glTexCoord2f(-.5,-.5);
   glVertex3f( -.5,-.5,0 );
   glTexCoord2f(.5,-.5);
   glVertex3f( .5, -.5,0 );
   glTexCoord2f(.5,.5);
   glVertex3f( .5, .5, 0 );
   glTexCoord2f(-.5,.5);
   glVertex3f( -.5,.5, 0 );
   //End quad
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}



void CMainMenuState::begin()
{
   SPosition2 size = Application.getWindowSize();
   float WindowWidth = size.X;
   float WindowHeight = size.Y;

   Camera = new CCamera(WindowWidth/WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);

   //glClearColor(1.0f,1.0f,1.0f,0);
   glClearColor(0,0,0,0);
   setupTextures();
   setupMeshes();
   our_font.init("WIFFLES_.TTF", 30);

   curAngle = 0;
   curDirection = 'l';

   Application.getSceneManager().addRenderable(renderLogo);
}
void CMainMenuState::end()
{
   printf("this ends it\n");
}

void CMainMenuState::OnRenderStart(float const Elapsed)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   
   glLoadIdentity();

   glDisable(GL_LIGHTING);

   glPushMatrix();

   drawSky(true);

   Application.get().getSceneManager().drawAll();

   SDL_GetMouseState(&mouse_x, &mouse_y);

   glTranslatef(.04f, .11, 0);

   if(mouse_x < 540 && mouse_x > 280 && mouse_y >225 && mouse_y < 300)
   {
      if (curDirection == 'l' && curAngle <= 20) {
         glPushMatrix();
         glRotatef(curAngle+=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -20) {
         glPushMatrix();
         glRotatef(curAngle-=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

      glPushMatrix();
      glLoadIdentity();
      glTranslatef(92, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8+.3*cos(curAngle/5),1);
      glTranslatef(-80,0,0);
     freetype::print(our_font, 320, 200, "New Game", curAngle);
     glPopMatrix();
   }

   else {
      drawButton();
      freetype::print(our_font, 332, 325.f, "New Game");
   }

   glTranslatef(0, -0.35, 0);

   if(mouse_x < 540 && mouse_x > 280 && mouse_y > 330 && mouse_y < 400)
   {
      if (curDirection == 'l' && curAngle <= 20) {
         glPushMatrix();
         glRotatef(curAngle+=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -20) {
         glPushMatrix();
         glRotatef(curAngle-=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

     glPushMatrix();
      glLoadIdentity();
      glTranslatef(102, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8+.3*cos(curAngle/5),1);
      glTranslatef(-90,0,0);
     freetype::print(our_font, 300, 95, "Stage Editor", curAngle);
     glPopMatrix();
   }

   else {
      drawButton();
      freetype::print(our_font, 310, 220.f, "Stage Editor");
   }

   glTranslatef(0, -0.35, 0);


   if(mouse_x < 540 && mouse_x > 280 && mouse_y > 435 && mouse_y < 510)
   {
      if (curDirection == 'l' && curAngle <= 20) {
         glPushMatrix();
         glRotatef(curAngle+=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'r';
      }

      if (curDirection == 'r' && curAngle >= -20) {
         glPushMatrix();
         glRotatef(curAngle-=.1, 0, 0, 1);
         drawButton();
         glPopMatrix();
      }

      else {
         curDirection = 'l';
      }

     glPushMatrix();
      glLoadIdentity();
      glTranslatef(90, 125, 0);
      glRotatef(curAngle,0,0,1);
      glScalef(1,.8+.3*cos(curAngle/5),1);
      glTranslatef(-90,0,0);
     freetype::print(our_font, 325, -10, "Exit Game", curAngle);
     glPopMatrix();

   }

   else {
      drawButton();
      freetype::print(our_font, 325, 115.f, "Exit Game");
   }

   //Check if should reset curAngle
   if (mouse_x >= 540 || mouse_x <= 280 || mouse_y >= 510 || mouse_y <= 225
      || (mouse_y <= 435 && mouse_y >= 400) || (mouse_y <= 330 && mouse_y >= 300))
   {
      curAngle = 0;
      curDirection = 'l';
   }




   glPopMatrix();
   

   glEnable(GL_LIGHTING);

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
               printf("mid button hit (Before)!!\n");
               CApplication::get().getStateManager().setState(& CLWIBState::get());
               printf("mid button hit!!\n");
            } 
            else if(Event.Location.Y >435  && Event.Location.Y < 510)
            {
               printf("Exiting program.\n");
               exit(1);
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
