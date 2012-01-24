#include <iostream>
#include <cmath>
#include <cstdlib>
#include "../CabbageCollider/CEngine.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeType.h"
#include <GL/glut.h>

//Chris Code
#include "3dsloader/3dsloader.h"
obj_type object;

void drawTree() {

   glPushMatrix();
   glTranslatef(-5, 1.5, 0);
   glRotatef(-90, 1, 0, 0);
   glScalef(2.0, 2.0, 2.0);
   glBegin(GL_TRIANGLES);

    for (int l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);

        //----------------- THIRD VERTEX -----------------
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }

   glEnd();
   glPopMatrix();
}


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

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player;
CObject *Floor, *Block;

//draw ground Plane for world
void drawPlane() {
    glPushMatrix();
    glColor3f(0.3, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex3f(-25, 0, -2.5);
        glVertex3f(25, 0, -2.5);
        glVertex3f(25, 0, 2.5);
        glVertex3f(-25, 0, 2.5);
    glEnd();

    glColor3f(0, 0, 0);
    glPointSize(15.f);
    glBegin(GL_LINES);
    for(float i = -25; i < 25; i += 0.5) {
       glVertex3f(i, 0.05, -2.5);
       glVertex3f(i, 0.05, 2.5);
       if(i <= 2.5 && i >= -2.5) {
          glVertex3f(-25, 0.05, i);
          glVertex3f(25, 0.05, i);
       }
    }
    glEnd();
    glPopMatrix();
}

void drawBlock() {
    //Block->setArea(SRect2(-15, -1, 1, 5));
    glPushMatrix();
       glColor3f(0, 0, 0);
       glTranslatef(-14, 0, 0);
       glScalef(2, 2, 1);
       glutSolidCube(1);
    glPopMatrix();
}

void EngineInit( void ) {
   Engine = new CEngine();
   Player = Engine->addActor();
   Player->setArea(SRect2(0, 0, 1, 1));

   Floor = Engine->addObject();
   Floor->setArea(SRect2(-25, -1, 50, 1));

   Block = Engine->addObject();
   Block->setArea(SRect2(-15, -1, 2, 2));
}


// Manages time independant movement and draws the VBO
void Display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
   SVector2 middleOfPlayer =
     SVector2(Player->getArea().Position.X + Player->getArea().Size.X/2,
              Player->getArea().Position.Y + Player->getArea().Size.Y/2);

	gluLookAt(
         middleOfPlayer.X, middleOfPlayer.Y + 1.3, 6,
         middleOfPlayer.X, middleOfPlayer.Y, 0,
      //gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y + 1, gameObjs[0].bunny.position.Z + 5, 
      //gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y, gameObjs[0].bunny.position.Z, 
		0, 1, 0);

      // draw the ground plane
      glDisable(GL_LIGHTING);
      drawPlane();
      glEnable(GL_LIGHTING);
      drawBlock();
      //Chris Code
      drawTree();


		glPushMatrix();
      glColor3f(0, 1, 1);
      glTranslatef(middleOfPlayer.X, middleOfPlayer.Y, 0);
		
      glutSolidSphere(0.5, 10, 10);
		//glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
		glPopMatrix();


	 // ...and by spinning it around
	 static float const RotationSpeed = 50.f;
    //Rotation.X += RotationSpeed*Delta;
    //Rotation.Y += RotationSpeed*Delta*2;
    glLoadIdentity();
    freetype::print(our_font, 10, SCREEN_HEIGHT-20, "Elapsed Time: %u\nNumber of Bunnies: %d\nDead Bunnies: %d ",elapsedTime/1000, numBunnies, numDeadBunnies );


    SDL_GL_SwapBuffers();
}

// OpenGL initialization
bool InitializeOGL()
{
	glClearColor(0.52f, 0.8f, 0.9f, 1.0f);

   //Chris Code
   Load3DS(&object, "3dsloader/tree.3ds");
 	
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


void DemoLight(void)
{
  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);

  // Light model parameters:
  // -------------------------------------------

  GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

  //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);


  // -------------------------------------------
  // Lighting parameters:

  GLfloat light_pos[] = {0.0f, 20.0f, 0.0f, 1.0f};
  GLfloat light_Ka[]  = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat light_Kd[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

  // -------------------------------------------
  // Material parameters:

  GLfloat material_Ka[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
  GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
  GLfloat material_Ke[] = {0.0f, 0.0f, 0.0f, 0.0f};
  GLfloat material_Se = 20.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
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
    DemoLight();

    int aDown, dDown;
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
      SVector2 Accel = Player->getAcceleration();
      Accel.X = 0;

      static float const MoveAccel = 5.8f;
      static float const JumpSpeed = 5.f;
      static float const AirMod = 0.25f;


      //printf("Player->isStanding(): %d\n", Player->isStanding());
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
             aDown = 1;
          }
          if(event.key.keysym.sym == SDLK_d){
             dDown = 1;
          }
          if(event.key.keysym.sym == SDLK_SPACE && Player->isStanding()) {
             Player->setVelocity(SVector2(Player->getVelocity().X, JumpSpeed));
          }
        } 

        if(event.type == SDL_KEYUP){
          if(event.key.keysym.sym == SDLK_w){
          }
          if(event.key.keysym.sym == SDLK_s){
          }
          if(event.key.keysym.sym == SDLK_a){
             aDown = 0;
          }
          if(event.key.keysym.sym == SDLK_d){
             dDown = 0;
          }
        }
        /*
        if(event.type == SDL_MOUSEMOTION)
        {
        }
        */
      }

      if(aDown) {
          Accel.X -= MoveAccel * (Player->isStanding() ? 1 : AirMod);
      }
      if(dDown) {
          Accel.X += MoveAccel * (Player->isStanding() ? 1 : AirMod);
      }

      Player->setAcceleration(Accel);
      Engine->updateAll((float)delta/1000); //Might be an issue (since updateAll requires float and delta is a UInt32)


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
