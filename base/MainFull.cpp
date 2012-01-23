#include <iostream>
#include <cmath>
#include <cstdlib>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeType.h"
#include <GL/glut.h>
#include "CCabbageColliderEngine.h"


// Portable version of system("PAUSE")
void waitForUser() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

/********************
 * Global Varaibles *
 ********************/
freetype::font_data our_font;

// Window information
//int WindowWidth = 400, WindowHeight = 400;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const float PI = 3.1415926535;

int numBunnies = 0;
int numDeadBunnies = 0;

Uint32 elapsedTime = 0;

// Time-independant movement variables
int Time0, Time1;
float ScaleTimer;

//Event handler
SDL_Event event;

float randFloat()
{
 return ((float)rand()/(float)RAND_MAX) * 2 - 1;
}

float randLimitedFloat()
{
 return ((float)rand()/(float)RAND_MAX) * 20 - 10;
}

// draw an individual square
void drawSquare() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
}

//draw ground Plane for world
void drawPlane() {
    glPushMatrix();

    for(float i = -25; i < 25; i += 0.5) {
        for (float j = -25; j < 25; j += 0.5) {
            glPushMatrix();
                glTranslatef(i, -1, j);
                glScalef(0.45, 0.45, 0.45);
                glRotatef(90.0, 1.0, 0.0, 0.0);
                drawSquare();
            glPopMatrix();
        }
    }
    glPopMatrix();
}

// OpenGL initialization
bool InitializeOGL()
{
	glClearColor(0.52f, 0.8f, 0.9f, 1.0f);

 	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glViewport( 0, 0, (GLsizei)(SCREEN_WIDTH), (GLsizei)(SCREEN_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)(SCREEN_WIDTH)/ (float)(SCREEN_HEIGHT), 0.01, 100.0);

    if( glGetError() != GL_NO_ERROR)
    {
      return false;
    }

    return true;

}

//Initialize SDL
bool StartSDLnOGL()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    return false;
  }

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  //Create Window
  if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_OPENGL | SDL_GL_DOUBLEBUFFER ) == NULL )
  {
    return false;
  }

  //Initialize OpenGL
  if(!InitializeOGL())
  {
    return false;
  }

  //Set caption
  SDL_WM_SetCaption( "Program 1 GO!!!", NULL );

  return true;
}

using namespace CabbageCollider;
CEngine *Engine;
CActor *Player;
CObject *Floor;

// Manages time independant movement and draws the VBO
void Display()
{
	/*// Determine time since last draw
	Time1 = glutGet(GLUT_ELAPSED_TIME);
	float Delta = (float) (Time1 - Time0) / 1000.f;
	Time0 = Time1;
    */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
      Player->getArea().Position.X, Player->getArea().Position.Y + 1, 5,
      Player->getArea().Position.X, Player->getArea().Position.Y, 0,
      //gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y + 1, gameObjs[0].bunny.position.Z + 5, 
      //gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y, gameObjs[0].bunny.position.Z, 
		0, 1, 0);

    // draw the ground plane
    drawPlane();

	 // ...and by spinning it around
	 static float const RotationSpeed = 50.f;
    //Rotation.X += RotationSpeed*Delta;
    //Rotation.Y += RotationSpeed*Delta*2;
    glLoadIdentity();
    freetype::print(our_font, 10, SCREEN_HEIGHT-20, "Elapsed Time: %u\nNumber of Bunnies: %d\nDead Bunnies: %d ",elapsedTime/1000, numBunnies, numDeadBunnies );


    SDL_GL_SwapBuffers();
}

//TODO support screen reshaping
void Reshape(int width, int height)								
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
//	WindowWidth = width;
//	WindowHeight = height;

	// Set camera projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	float AspectRatio = (float)WindowWidth / (float)WindowHeight;
}

void EngineInit( void ) {
   Engine = new CEngine();
   Player = Engine->addActor();
   Player->setArea(SRect2(0, 0, 0.3, 0.3));

   Floor = Engine->addObject();
   Floor->setArea(SRect2(-10, -10, 10, 10));
}

int main(int argc, char * argv[])
{

    glutInit(&argc, argv);
  if(!StartSDLnOGL())
  {
		std::cerr << "openGL or SDL failed to init" << std::endl;
    return 1;
  }

  srand(time(NULL));

  our_font.init("pirulen.ttf", 16);

    // Create new gameObject
    numBunnies++;

    bool finished = false;
    Uint32 start = SDL_GetTicks();
    Uint32 end = start;
    Uint32 delta;
    Uint32 spawnTimer = 0;
    Uint32 spawnRate = 7500;
    Uint32 finalTime = 60000;
    double dDeltaSec;
    double phi = 0.0;
    double theta = -1.570796327;
    GLdouble x = 0.0;
    GLdouble y = 0.0;
    GLdouble z = 0.0;

    EngineInit();

    while(!finished)
    {
      //fps.start();
      do {
        end = SDL_GetTicks();
      } while(start == end);

      delta = end - start;
      dDeltaSec = (double) delta / 1000.0;
      start = end;
      elapsedTime += delta;

      if(elapsedTime >= finalTime){
        finished = true;
        continue;
      }

      /*
      spawnTimer += delta;
      if(spawnTimer > spawnRate)
      {
        // Create new gameObject
        gameObject obj1 = {Bunny(randLimitedFloat(), randLimitedFloat(), randFloat(), randFloat()), SVector3(), SVector3()};


        gameObjs.push_back(obj1);
        spawnTimer -= spawnRate;
        numBunnies++;
      }
      */

      SVector2 Accel = Player->getAcceleration();
      Accel.X = 0;

      static float const MoveAccel = 5.8f;
      static float const JumpSpeed = 5.f;
      static float const AirMod = 0.25f;


      while(SDL_PollEvent( &event ) )
      {
        if(event.type == SDL_QUIT )
        {
          finished = true;
        }

        if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_w){
          }
          if(event.key.keysym.sym == SDLK_s){
          }
          if(event.key.keysym.sym == SDLK_a){
             Accel.X += MoveAccel * (Player->isStanding() ? 1 : AirMod);
             //gameObjs[0].bunny.direction.X -= 0.01;
          }
          if(event.key.keysym.sym == SDLK_d){
             Accel.X -= MoveAccel * (Player->isStanding() ? 1 : AirMod);
             //gameObjs[0].bunny.direction.X += 0.01;
          }
        } 

/*
        if(event.type == SDL_KEYUP){
          if(event.key.keysym.sym == SDLK_w){
          }
          if(event.key.keysym.sym == SDLK_s){
          }
          if(event.key.keysym.sym == SDLK_a){
             gameObjs[0].bunny.direction.X += 0.01;
          }
          if(event.key.keysym.sym == SDLK_d){
             gameObjs[0].bunny.direction.X -= 0.01;
          }
        }
*/

        /*
        if(event.type == SDL_MOUSEMOTION)
        {
        }
        */
      }

      Player->setAcceleration(Accel);
      Engine->updateAll((float)delta); //Might be an issue (since updateAll requires float and delta is a UInt32)


      /*
      for(int i = 0; i < gameObjs.size(); i++)
      {
        gameObjs[i].bunny.step((float) delta);
      }
      */

      /*
      loopBunnyCollision(gameObjs.size());
      playerCollision(gameObjs.size());
      */

      //std::cout << "The Ticks: " << ((float) delta / 1000.f) << std::endl ;
      Display();

		glPushMatrix();
      glTranslatef(Player->getArea().Position.X, Player->getArea().Position.Y, 0);
		
      glutSolidSphere(0.5, 10, 10);
		//glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
		glPopMatrix();

      //Cap the frame rate
      /*
      if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
      {
        SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
      }
      */

    }
    our_font.clean();

    SDL_Quit();
	
	return 0;
}
