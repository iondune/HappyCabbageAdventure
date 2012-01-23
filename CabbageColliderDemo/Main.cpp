//! STL and other StdLib files
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageCollider.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif


#include "../CabbageCollider/CCabbageColliderEngine.h"
using namespace CabbageCollider;


void Initialize()
{
	glClearColor (0.9f, 0.6f, 0.3f, 1.0f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.01, 100.0);
}

CActor * Player;

void Draw()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		Player->getArea().Position.X, Player->getArea().Position.Y, 8, 
		Player->getArea().Position.X, Player->getArea().Position.Y, 0, 
		0, 1, 0);

	glPointSize(10.f);
	glBegin(GL_POINTS);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 5.f, 0.f);
		glVertex3f(0.f, 10.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(5.f, 0.f, 0.f);
		glVertex3f(10.f, 0.f, 0.f);
	glEnd();
}

int main(int argc, char * argv[])
{
	int const WindowWidth = 800, WindowHeight = 600;
	SDL_VideoInfo const * video;

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
        
    /* Quit SDL properly on exit */
    atexit(SDL_Quit);

    /* Get the current video information */
    video = SDL_GetVideoInfo( );
    if( video == NULL )
	{
        fprintf(stderr, "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    }

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* Note the SDL_DOUBLEBUF flag is not required to enable double 
     * buffering when setting an OpenGL video mode. 
     * Double buffering is enabled or disabled using the 
     * SDL_GL_DOUBLEBUFFER attribute.
     */
    if( SDL_SetVideoMode( WindowWidth, WindowHeight, video->vfmt->BitsPerPixel, SDL_OPENGL ) == 0 )
	{
        fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    }

	// Load OpenGL Extensions
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing glew! (%s)\n", glewGetErrorString(err));
		system("PAUSE");
		return 1;
	}

   	Initialize();


	bool IsKeyDown[SDLK_LAST];
	for (unsigned int i = 0; i < SDLK_LAST; ++ i)
		IsKeyDown[i] = false;

	CEngine * Engine = new CEngine();
	Player = Engine->addActor();
	Player->setArea(SRect2(0, 3, 1, 1));

	CObject * Block = Engine->addObject();
	Block->setArea(SRect2(-1, -1, 15, 0.9f));

	CObject * Block2 = Engine->addObject();
	Block2->setArea(SRect2(2, 2, 1, 1.f));

	// Time-independant movement variables
	int Time0, Time1;

	// Start keeping track of time
	Time0 = SDL_GetTicks();

  	while (1)
	{
        /* process pending events */
		SDL_Event event;
        while( SDL_PollEvent( &event ) )
		{
            switch( event.type )
			{

            case SDL_QUIT:
                exit (0);
                break;

			case SDL_KEYDOWN:
				IsKeyDown[event.key.keysym.sym] = true;
				break;

			case SDL_KEYUP:
				IsKeyDown[event.key.keysym.sym] = false;
				break;

            }
        }

		// Determine time since last draw
		Time1 = SDL_GetTicks();
		float Delta = (float) (Time1 - Time0) / 1000.f;
		Time0 = Time1;

		SVector2 Accel = Player->getAcceleration();
		Accel.X = 0;
		static float const MoveAccel = 5.8f;
		static float const JumpSpeed = 5.f;
		static float const AirMod = 0.25f;

		if (IsKeyDown[SDLK_d])
		{
			Accel.X += MoveAccel * (Player->isStanding() ? 1 : AirMod);
		}

		if (IsKeyDown[SDLK_a])
		{
			Accel.X -= MoveAccel * (Player->isStanding() ? 1 : AirMod);
		}

		if (IsKeyDown[SDLK_SPACE] && Player->isStanding())
		{
			Player->setVelocity(SVector2(Player->getVelocity().X, JumpSpeed));
		}

		Player->setAcceleration(Accel);

		Engine->updateAll(Delta);

		Draw();

		glPushMatrix();
		glTranslatef(Player->getArea().Position.X, Player->getArea().Position.Y, 0);

		if (Player->isStanding())
			glColor3f(1.0, 0, 0);
		else
			glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(1.f, 0.f, 0.f);
			glVertex3f(1.f, 1.f, 0.f);
			glVertex3f(0.f, 1.f, 0.f);
		glEnd();
		glPopMatrix();

		glColor3f(1, 1, 1);

		glPushMatrix();
		glTranslatef(Block->getArea().Position.X, Block->getArea().Position.Y, 0);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(15.f, 0.f, 0.f);
			glVertex3f(15.f, 0.9f, 0.f);
			glVertex3f(0.f, 0.9f, 0.f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(Block2->getArea().Position.X, Block2->getArea().Position.Y, 0);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(1.f, 0.f, 0.f);
			glVertex3f(1.f, 1.f, 0.f);
			glVertex3f(0.f, 1.f, 0.f);
		glEnd();
		glPopMatrix();

		SDL_GL_SwapBuffers();
	}

   	return 0;
}

