#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Util/SVector3.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeType.h"
#include <GL/glut.h>

// Utility classes for loading shaders/meshes
#include "CMeshLoader.h"
#include "CShader.h"
#include "Bunny.h"

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

// Shader and Mesh utility classes
CShader * Shader;
CMesh * Mesh;

// Information about mesh
int TriangleCount;
// Handles for VBOs
GLuint PositionBufferHandle, ColorBufferHandle;

// Window information
//int WindowWidth = 400, WindowHeight = 400;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const float PI = 3.1415926535;

GLdouble eyeX = 0.0;
GLdouble eyeY = 0.0;
GLdouble eyeZ = 2.0;
GLdouble centerX = 0.0;
GLdouble centerY = 0.0;
GLdouble centerZ = 0.0;
GLdouble upX = 0.0;
GLdouble upY = 1.0;
GLdouble upZ = 0.0;

GLdouble eyeXVelo = 0.0;
GLdouble eyeYVelo = 0.0;
GLdouble eyeZVelo = 0.0;
GLdouble centerXVelo = 0.0;
GLdouble centerYVelo = 0.0;
GLdouble centerZVelo = 0.0;

GLdouble tz = 0.0;
GLdouble tx = 0.0;

int numBunnies = 0;
int numDeadBunnies = 0;

Uint32 elapsedTime = 0;

typedef struct gameObject {
    Bunny bunny;
    SVector3 box;
    SVector3 center;
} gameObject;  

Bunny player;
// Time-independant movement variables
int Time0, Time1;
float ScaleTimer;

std::vector<gameObject> gameObjs;
//Event handler
SDL_Event event;

//
// Normalize a vector
//
SVector3 normalizeVector(SVector3 v){
    float x = v.X / v.length();
    float y = v.Y / v.length();
    float z = v.Z / v.length();
    SVector3 normal = SVector3(x, y, z);
    return normal;
}

//
// Collision Version 2
//
bool checkBunnyCollision(Bunny b1, Bunny b2)
{
    if (b1.AABBmin.X > b2.AABBmax.X) 
        return false;
    if (b1.AABBmin.Y > b2.AABBmax.Y)
        return false;
    if (b1.AABBmin.Z > b2.AABBmax.Z)
        return false;
    if (b1.AABBmax.X < b2.AABBmin.X)
        return false;
    if (b1.AABBmax.Y < b2.AABBmin.Y)
        return false;
    if (b1.AABBmax.Z < b2.AABBmin.Z)
        return false;

    std::cout << "happening" << std::endl ;
    return true;
}

//
// AABB Collision Detection
//
bool singleCollide(gameObject obj1, gameObject obj2) {
    if (fabs(obj1.center.X - obj2.center.X) > (obj1.box.X - obj2.box.X))
       return false;
    if (fabs(obj1.center.Y - obj2.center.Y) > (obj1.box.Y - obj2.box.Y))
       return false;
    if (fabs(obj1.center.Z - obj2.center.Z) > (obj1.box.Z - obj2.box.Z))
       return false;
    
    //collided
    return true; 
}

//
// AABB Collision mega check 
//
bool collided(gameObject obj1, gameObject obj2) {
    if (singleCollide(obj1, obj2) || singleCollide(obj2, obj1))
       return true;
    else
       return false; 
}

float randFloat()
{
 return ((float)rand()/(float)RAND_MAX) * 2 - 1;
}

float randLimitedFloat()
{
 return ((float)rand()/(float)RAND_MAX) * 20 - 10;
}
//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

Timer fps;

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
      gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y + 1, gameObjs[0].bunny.position.Z + 5, 
      //Overhead cam: gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y + 10, gameObjs[0].bunny.position.Z + 0.1, 
      gameObjs[0].bunny.position.X, gameObjs[0].bunny.position.Y, gameObjs[0].bunny.position.Z, 
		//centerX, centerY, centerZ, 
		upX, upY, upZ);

	// Animates the loaded model by modulating it's size
	static float const ScaleSpeed = 1.f;
	static float const ScaleThreshold = 0.4f;
	//ScaleTimer += Delta * ScaleSpeed;
	//Scale = SVector3(1) + ScaleThreshold * cos(ScaleTimer);


    // draw the ground plane
    drawPlane();

    for(int i = 0; i < gameObjs.size(); i++)
    {
      gameObjs[i].bunny.draw(Shader, PositionBufferHandle, ColorBufferHandle, TriangleCount);
    }

	// ...and by spinning it around
	static float const RotationSpeed = 50.f;
	//Rotation.X += RotationSpeed*Delta;
	//Rotation.Y += RotationSpeed*Delta*2;
    glLoadIdentity();
    freetype::print(our_font, 10, SCREEN_HEIGHT-20, "Elapsed Time: %u\nNumber of Bunnies: %d\nDead Bunnies: %d ",elapsedTime/1000, numBunnies, numDeadBunnies );


    SDL_GL_SwapBuffers();
	//glutSwapBuffers();
	//glutPostRedisplay();
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

void loopBunnyCollision(int size)
{
    Bunny go;
    for (int i = 0; i < size; i++) {
        go = gameObjs[i].bunny;
        if(go.stopped || go.hit)
            continue;

        for (int j = i+1; j < size; j++) {
            if(checkBunnyCollision(go, gameObjs[j].bunny))
            {
               go.stopped = true;
               gameObjs[i].bunny.stopped = true;
               gameObjs[j].bunny.stopped = true;
             std::cout << "bunny collison" << std::endl ;
            }
        }
    }
}

void playerCollision(int size)
{
    for (int i = 0; i < size; i++) {
      if(gameObjs[i].bunny.hit == true)
        continue;

      if(checkBunnyCollision(player, gameObjs[i].bunny))
      {
              gameObjs[i].bunny.hit = true;
              numDeadBunnies++;
             std::cout << "bunny hit" << std::endl ;
            }
    }
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

	// First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		waitForUser();
		return 1;
	}

	// Now attempt to load the shaders
	Shader = ShaderLoader.loadShader("Shaders/Lab3_vert.glsl", "Shaders/Lab3_frag.glsl");
	if (! Shader)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUser();
		return 1;
	}
	Shader->loadAttribute("aPosition");
	Shader->loadAttribute("aColor");

	// Attempt to load mesh
	CMesh * Mesh = CMeshLoader::loadASCIIMesh("Models/bunny500.m");
	if (! Mesh)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		waitForUser();
		return 1;
	}
	// Make out mesh fit within camera view
	Mesh->resizeMesh(SVector3(1));
	// And center it at the origin
	Mesh->centerMeshByExtents(SVector3(0));

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* Mesh, TriangleCount, PositionBufferHandle, ColorBufferHandle);
    
    // Create new gameObject
    gameObject obj = {Bunny(0, 0, 0, 0), SVector3(), SVector3()};

    gameObjs.push_back(obj);
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


      while(SDL_PollEvent( &event ) )
      {
        if(event.type == SDL_QUIT )
        {
          finished = true;
        }

        // convert center and eye to vectors
        SVector3 center = SVector3(centerX, centerY, centerZ);
        SVector3 eye = SVector3(eyeX, eyeY, eyeZ);

        // Normalize Vectors
        SVector3 v = center - eye;
        SVector3 u = v.crossProduct(SVector3(0.0, 1.0, 0.0));

        // inverse
        u.X = -u.X;
        u.Y = -u.Y;
        u.Z = -u.Z;

        SVector3 dv = normalizeVector(v) * 0.5f;  
        SVector3 du = normalizeVector(u) * 0.5f;
        
        SVector3 nextPos = SVector3();
        SVector3 nextOrientation = SVector3();

        /*
        if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_w){
            eyeXVelo += (GLdouble)dv.X;
            eyeYVelo  += (GLdouble)dv.Y;
            eyeZVelo  += (GLdouble)dv.Z;
            centerXVelo  += (GLdouble)dv.X;
            centerYVelo  += (GLdouble)dv.Y;
            centerZVelo  += (GLdouble)dv.Z;
          }
          if(event.key.keysym.sym == SDLK_s){
            eyeXVelo  -= (GLdouble)dv.X;
            eyeYVelo  -= (GLdouble)dv.Y;
            eyeZVelo  -= (GLdouble)dv.Z;
            centerXVelo  -= (GLdouble)dv.X;
            centerYVelo  -= (GLdouble)dv.Y;
            centerZVelo  -= (GLdouble)dv.Z;
          }
          if(event.key.keysym.sym == SDLK_a){
            eyeXVelo  += (GLdouble)du.X;
            eyeYVelo  += (GLdouble)du.Y;
            eyeZVelo  += (GLdouble)du.Z;
            centerXVelo  += (GLdouble)du.X;
            centerYVelo  += (GLdouble)du.Y;
            centerZVelo  += (GLdouble)du.Z;
          }
          if(event.key.keysym.sym == SDLK_d){
            eyeXVelo -= (GLdouble)du.X;
            eyeYVelo -= (GLdouble)du.Y;
            eyeZVelo -= (GLdouble)du.Z;
            centerXVelo -= (GLdouble)du.X;
            centerYVelo -= (GLdouble)du.Y;
            centerZVelo -= (GLdouble)du.Z;
          }
        } 
        */

        if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_w){
          }
          if(event.key.keysym.sym == SDLK_s){
          }
          if(event.key.keysym.sym == SDLK_a){
             printf("Being called A: xpos = %0.2f \n", gameObjs[0].bunny.position.X);
             gameObjs[0].bunny.direction.X -= 0.01;
          }
          if(event.key.keysym.sym == SDLK_d){
             printf("Being called D: xpos = %0.2f \n", gameObjs[0].bunny.position.X);
             gameObjs[0].bunny.direction.X += 0.01;
          }
        } 

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


        /*
        if(event.type == SDL_KEYUP){

          if(event.key.keysym.sym == SDLK_a
             || event.key.keysym.sym == SDLK_s
             || event.key.keysym.sym == SDLK_d
             || event.key.keysym.sym == SDLK_w){
            eyeXVelo = 0.0;
            eyeYVelo = 0.0;
            eyeZVelo = 0.0;
            centerXVelo = 0.0;
            centerYVelo = 0.0;
            centerZVelo = 0.0;
          }
          if(event.key.keysym.sym == SDLK_HOME)
          {
            eyeX = 0.0;
            eyeY = 0.0;
            eyeZ = 2.0;
            centerX = 0.0;
            centerY = 0.0;
            centerZ = 1.0;
          }
          */
        /*
          if(event.key.keysym.sym == SDLK_w){
            eyeXVelo -= (GLdouble)dv.X;
            eyeYVelo -= (GLdouble)dv.Y;
            eyeZVelo -= (GLdouble)dv.Z;
            centerXVelo -= (GLdouble)dv.X;
            centerYVelo -= (GLdouble)dv.Y;
            centerZVelo -= (GLdouble)dv.Z;
          }
          if(event.key.keysym.sym == SDLK_s){
            eyeXVelo += (GLdouble)dv.X;
            eyeYVelo += (GLdouble)dv.Y;
            eyeZVelo += (GLdouble)dv.Z;
            centerXVelo += (GLdouble)dv.X;
            centerYVelo += (GLdouble)dv.Y;
            centerZVelo += (GLdouble)dv.Z;
          }
          if(event.key.keysym.sym == SDLK_a){
            eyeXVelo -= (GLdouble)du.X;
            eyeYVelo -= (GLdouble)du.Y;
            eyeZVelo -= (GLdouble)du.Z;
            centerXVelo -= (GLdouble)du.X;
            centerYVelo -= (GLdouble)du.Y;
            centerZVelo -= (GLdouble)du.Z;
          }
          if(event.key.keysym.sym == SDLK_d){
            eyeXVelo += (GLdouble)du.X;
            eyeYVelo += (GLdouble)du.Y;
             eyeZVelo += (GLdouble)du.Z;
            centerXVelo += (GLdouble)du.X;
            centerYVelo += (GLdouble)du.Y;
            centerZVelo += (GLdouble)du.Z;
          }
        } 
        */

        if(event.type == SDL_MOUSEMOTION)
        {
          if(event.motion.xrel > 1.0)
          {
            theta += asin((double)0.025);
          }
          else if( event.motion.xrel < -1.0)
          {
            theta += asin((double)-0.025);
          }
          else
          {
            theta += asin((double)event.motion.xrel * 0.025) ;
          }

          if(event.motion.yrel * -1.0 > 1.0)
          {
            phi += asin((double)0.025);
          } 
          else if( event.motion.yrel * -1.0 < -1.0)
          {
            phi += asin((double)-0.025);
          }
          else
          {
            phi += asin((double)event.motion.yrel * -0.025);
          }

          //theta = theta > 2.0 * PI ? 2.0 * PI : theta;
          

        //     std::cout << "The Theta: " << theta << std::endl ;
          //   std::cout << "The Phi: " << phi << std::endl ;

          centerX = eyeX + (GLdouble) (cos(phi) * cos(theta));
          centerY = eyeY + (GLdouble) sin(phi);
          centerZ = eyeZ + (GLdouble) (cos(phi) * cos(1.570796327 - theta));
             //std::cout << "The :x " << centerX << std::endl ;
             //std::cout << "The :y " << centerY << std::endl ;
             //std::cout << "The :z " << centerZ << std::endl ;
        }
      }

            eyeX += eyeXVelo * dDeltaSec * 4.5;
            eyeY += eyeYVelo * dDeltaSec * 4.5;
            eyeZ += eyeZVelo * dDeltaSec * 4.5;
            centerX += centerXVelo * dDeltaSec * 4.5;
            centerY += centerYVelo * dDeltaSec * 4.5;
            centerZ += centerZVelo * dDeltaSec * 4.5;

            eyeY = eyeY < 0.0 ? 0.0 : eyeY;

      for(int i = 0; i < gameObjs.size(); i++)
      {
        gameObjs[i].bunny.step((float) delta);
      }

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
