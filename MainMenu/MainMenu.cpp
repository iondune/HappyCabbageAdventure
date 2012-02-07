#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "../CabbageCore/CabbageCore.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageScene/CabbageScene.h"

#include "../base/texture.h"


class CMainMenuState : public CState<CMainMenuState>
{

public:

  int woodTexture, skyTexture;

  void setupTextures()
  {
    woodTexture = LoadBitmap("wood011.bmp");
    skyTexture = LoadBitmap("sky.bmp");
  }

  void drawSky(int backwards) {
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, skyTexture);

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

  void drawButton()
  {
      glEnable(GL_TEXTURE_2D);
      glPushMatrix();

      glBindTexture(GL_TEXTURE_2D, woodTexture);

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

    virtual void begin()
    {
      printf("this begins it\n");
      setupTextures();
    }
    virtual void end()
    {
      printf("this ends it\n");
    }


    virtual void OnRenderStart(float const Elapsed)
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
    virtual void OnRenderEnd(float const Elapsed)
    {
    
    }


    virtual void OnMouseEvent(SMouseEvent const & Event)
    {
      if(Event.Type.Value == SMouseEvent::EType::Move){
        printf("this is mouse move: Location x %d y %d\n",Event.Location.X, Event.Location.Y);
      }
      else if(Event.Type.Value == SMouseEvent::EType::Click){
        printf("this is mouse click: Location x %d y %d ",Event.Location.X, Event.Location.Y);
        printf(" Pressed %d ", Event.Pressed);

        switch (Event.Button.Value){

        case SMouseEvent::EButton::Left:
          printf(" Left\n");

          if(!Event.Pressed && Event.Location.X < 540 && Event.Location.X > 280)
          {
            if(Event.Location.Y >225 && Event.Location.Y < 300)
            {
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
          printf(" Right\n", Event.Pressed);
          break;
        case SMouseEvent::EButton::Middle:
          printf(" Middle\n", Event.Pressed);
          break;
        }


      }
    }
    virtual void OnKeyboardEvent(SKeyboardEvent const & Event)
    {
      printf("this is key\n");
    }

};

int main(int argc, char * argv[])
{
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(800, 600));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState(& CMainMenuState::get());

    Application.run();

    return 0;
}

