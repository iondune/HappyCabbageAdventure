#include "header.h"

#define TREE_Y_OFFSET 2.1
void addTrees(int numTrees) {
   for (int n = 0; n < numTrees; n++) {
      glPushMatrix();
      glTranslatef(-25 + n * 5, TREE_Y_OFFSET, -2);
      drawTree();
      glPopMatrix();
      if(n % 3 == 0) {
         glPushMatrix();
         glTranslatef(-22 + n * 4.73, TREE_Y_OFFSET, 2);
         drawTree();
         glPopMatrix();
      }
   }
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
const float PI = 3.14159f;

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

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player, *Derp;
CObject *Floor, *Block;
CPlayerView *PlayerView;

std::vector<SRect2> blocks;
void EngineInit( void ) {
   Engine = new CEngine();
   Player = Engine->addActor();
   Player->setArea(SRect2(-24.5, 3, 1, 1));

   Derp = Engine->addActor();
   Derp->setArea(SRect2(-17, 0, 1, 1));

   Floor = Engine->addObject();
   Floor->setArea(SRect2(-25, -1, 50, 1));

   SRect2 area;

   int i = 0;
   float j = 0;

   for(j = 0; j < 10; j+=2.5) {
      Block = Engine->addObject();
      area = SRect2(-15 + j, 1.5 + j, 2, 1);
      Block->setArea(area);
      blocks.push_back(area);
   }

   Block = Engine->addObject();
   area = SRect2(-22, 7, 6, 0.2);
   Block->setArea(area);
   blocks.push_back(area);

   Block = Engine->addObject();
   area = SRect2(-22, 7, 0.2, 3);
   Block->setArea(area);
   blocks.push_back(area);

   for(; i < 12; i++) {
      Block = Engine->addObject();
      area = SRect2(-i + 5, 0, 1, i+1);
      Block->setArea(area);
      blocks.push_back(area);
   }

}


void ViewInit( void ) {
   PlayerView = new CPlayerView();
}


double fps = 0.0;
// Manages time independant movement and draws the VBO
void Display()
{
   Uint32 startclock = SDL_GetTicks();
   Uint32 currentFPS = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   SVector2 middleOfPlayer = Player->getArea().getCenter();
   PlayerView->setMiddle(middleOfPlayer);
   PlayerView->setGround(Engine->getHeightBelow(Player));

	glMatrixMode(GL_MODELVIEW);
   PlayerView->establishCamera();

      // draw the ground plane
      glDisable(GL_LIGHTING);
      drawPlane();
      drawSky();
      drawDirt();
      glEnable(GL_LIGHTING);
      setMaterial(BROWN_MATTE);

      std::vector<SRect2>::iterator itr;
      for (itr = blocks.begin(); itr < blocks.end(); ++itr) {
         SRect2 block = (*itr);
         drawBlock(block.Position.X, block.Position.Y, block.Size.X, block.Size.Y); 
      }
      
      //Chris Code, draw Trees
      addTrees(NUM_TREES);
      PlayerView->draw();

      //Draw derp (enemy)
		glPushMatrix();
      glColor3f(1, 0.6, 0);
      glTranslatef(Derp->getArea().getCenter().X, Derp->getArea().getCenter().Y, 0);
		
      glutSolidSphere(0.5, 10, 10);
		//glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
		glPopMatrix();


	 // ...and by spinning it around
	 static float const RotationSpeed = 50.f;
    //Rotation.X += RotationSpeed*Delta;
    //Rotation.Y += RotationSpeed*Delta*2;

    glLoadIdentity();

	if (SDL_GetTicks() != startclock)
		currentFPS = 1000.f / float(SDL_GetTicks() - startclock);
    freetype::print(our_font, 10, SCREEN_HEIGHT-40, "Elapsed Time: %u\n"
         "FPS: %0.0f ", elapsedTime/1000, fps);


    SDL_GL_SwapBuffers();
}

// OpenGL initialization
bool InitializeOGL()
{
	glClearColor(0.52f, 0.8f, 0.9f, 1.0f);

   //Chris Code
   Load3DS(&object, "3dsloader/tree.3ds");
   groundTexture = LoadBitmap("grass.bmp");
   skyTexture = LoadBitmap("sky.bmp");
   dirtTexture = LoadBitmap("dirt.bmp");
 	
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
  SDL_WM_SetCaption( "Happy Cabbage Adventure", NULL );

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
  GLfloat light_Kd[]  = {.8f, 1.0f, .8f, 1.0f};
  GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
}


Uint32 frame_delta = 0, frames = 0;

int main(int argc, char * argv[])
{

    glutInit(&argc, argv);
  if(!StartSDLnOGL())
  {
		std::cerr << "openGL or SDL failed to init" << std::endl;
    return 1;
  }

  srand(time(NULL));

  our_font.init("WIFFLES_.TTF", 30);

    // Create new gameObject

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
    ViewInit();
    DemoLight();

    int aDown = 0, dDown = 0, spaceDown = 0;
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

      //SDL_Delay((Uint32)100);

      //Better framerate calculation?
      //Still says 1000 on the lab machines
    frames++;
    Uint32 newTicks = SDL_GetTicks();
    if(newTicks - frame_delta >= 100) {
       //printf("%u\n", frames);
       fps = (float)frames / (((float)(newTicks - frame_delta))/1000);
       frames = 0;
       frame_delta = newTicks;
    }



      /*
      if(elapsedTime >= finalTime){
        finished = true;
        continue;
      }
      */


      //printf("Player->isStanding(): %d\n", Player->isStanding());
      while(SDL_PollEvent( &event ) )
      {
        if(event.type == SDL_QUIT)
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
          if(event.key.keysym.sym == SDLK_SPACE) {
             spaceDown = 1;
          }
          if (event.key.keysym.sym == SDLK_ESCAPE) {
             finished = true;
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
		  if (event.key.keysym.sym = SDLK_SPACE){
			  spaceDown = 0;
		  }
        }
        /*
        if(event.type == SDL_MOUSEMOTION)
        {
        }
        */
      }

      float curXVelocity = Player->getVelocity().X;
      PlayerView->setVelocity(Player->getVelocity());

	   if(dDown && aDown) {
	 	   Player->setAction(CActor::EActionType::None);
         PlayerView->setState(CPlayerView::State::Standing);
	   }
      else if(aDown) {
         Player->setAction(CActor::EActionType::MoveLeft);
         PlayerView->setState(CPlayerView::State::MovingLeft);
      }
      else if(dDown) {
         Player->setAction(CActor::EActionType::MoveRight);
         PlayerView->setState(CPlayerView::State::MovingRight);
      }
	   else {
         Player->setAction(CActor::EActionType::None);
         PlayerView->setState(CPlayerView::State::Standing);
	   }

		  Player->setJumping(spaceDown != 0);

      /*
      if(curXVelocity > 0.1) {
         PlayerView->setState(CPlayerView::State::MovingRight);
      }
      else if(curXVelocity < -0.1) {
         PlayerView->setState(CPlayerView::State::MovingLeft);
      }
      else if(Player->isStanding()){
         PlayerView->setState(CPlayerView::State::Standing);
      }
      */

	  Derp->setJumping(true);

      Engine->updateAll((float)delta/1000); //Might be an issue (since updateAll requires float and delta is a UInt32)
      PlayerView->step((float)delta);


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
