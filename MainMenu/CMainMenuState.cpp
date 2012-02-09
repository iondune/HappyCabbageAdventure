#include "CMainMenuState.h"

CMainMenuState::CMainMenuState() {}

void CMainMenuState::setupTextures()
{
   woodTexture = new CTexture(CImageLoader::loadImage("wood011.bmp"));
   skyTexture = new CTexture(CImageLoader::loadImage("sky.bmp"));
}

void CMainMenuState::setupMeshes()
{
  CShader *Flat;
  Flat = CShaderLoader::loadShader("Diffuse");
  logoMesh = CMeshLoader::load3dsMesh("../base/Models/HappyLogo2.3ds");
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
  CApplication::get().getSceneManager().addRenderable(renderLogo);



}

void CMainMenuState::drawSky(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, skyTexture->getTextureHandle());

   glTranslatef(0,0,1.01f);//sky not shown
   //glTranslatef(0,0,1.00f);//sky shown
   glBegin(GL_QUADS);
   if(!backwards) {
      glTexCoord2f(0, 1);
      glVertex3f(-25, 22, -2.5f);
      glTexCoord2f(0, 0);
      glVertex3f(-25, -1, -2.5f);
      glTexCoord2f(1, 0);
      glVertex3f(25, -1, -2.5f);
      glTexCoord2f(1, 1);
      glVertex3f(25, 22, -2.5f);
   }
   else {
      glTexCoord2f(0, 1);
      glVertex3f(-2, 2, 0.0f);
      glTexCoord2f(0, 0);
      glVertex3f(-2, -2, 0.0f);
      glTexCoord2f(1, 0);
      glVertex3f(2, -2, 0.0f);
      glTexCoord2f(1, 1);
      glVertex3f(2, 2, 0.0f);
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

   glScalef(0.65f, 0.25f, 1.f);

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
   //glClearColor(1.0f,1.0f,1.0f,0);
  CApplication & Application = CApplication::get();
   SPosition2 windowSize = Application.getWindowSize();

   Application.getSceneManager().setActiveCamera(new CCamera((float)windowSize.X/(float)windowSize.Y, 0.01f, 100.f, 60.f));

   glClearColor(0,0,0,0);
   setupTextures();
   setupMeshes();
   our_font.init("WIFFLES_.TTF", 30);
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

   glLoadIdentity();

   CApplication::get().getSceneManager().drawAll();
   glDisable(GL_LIGHTING);

   glPushMatrix();

   drawSky(true);

   glTranslatef(-0.3f,0,0);
   drawButton();
   glTranslatef(0,-0.35f, 0);
   drawButton();
   glTranslatef(0,-0.35f, 0);
   drawButton();

   glPopMatrix();
   

   glEnable(GL_LIGHTING);

   freetype::print(our_font, 330, 325.f, "New Game");
   freetype::print(our_font, 310, 220.f, "Stage Editor");
   freetype::print(our_font, 325, 115.f, "Exit Game");

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
