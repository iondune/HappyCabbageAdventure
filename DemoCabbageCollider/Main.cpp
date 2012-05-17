//! STL and other StdLib files
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


#ifdef __unix__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageCollider.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif


#include "../CabbageCollider/CCollisionEngine.h"


void Initialize()
{
	glClearColor (0.9f, 0.6f, 0.3f, 1.0f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.01, 100.0);
}

CCollisionActor * Player;

void Draw()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 8, 
		Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0, 
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
	int const WindowWidth = 1600, WindowHeight = 900;
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

#ifdef _WIN32
	// Load OpenGL Extensions
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing glew! (%s)\n", glewGetErrorString(err));
		system("PAUSE");
		return 1;
	}
#endif

   	Initialize();


	bool IsKeyDown[SDLK_LAST];
	for (unsigned int i = 0; i < SDLK_LAST; ++ i)
		IsKeyDown[i] = false;

	CCollisionEngine * Engine = new CCollisionEngine();
	Player = Engine->addActor();
	Player->setArea(SArea(0, 3, 1, 1));

	CCollisionObject * Block2 = Engine->addObject();
	Block2->setArea(SArea(2, 1.1f, 1, 1.f));

	CCollisionObject * Block = Engine->addObject();
	Block->setArea(SArea(-1, -1, 15, 0.9f));
	Block->getMaterial().Friction = 0.4f;
	Block = Engine->addObject();
	Block->setArea(SArea(-6.5, -1, 5, 0.9f));
	Block->getMaterial().Friction = 0.4f;

    CCollisionElevator * Elevator = Engine->addElevator();
    Elevator->setArea(SArea(6, -3, 1, 1));
	//Elevator->InitialDirection *= -1;

    Elevator = Engine->addElevator();
    Elevator->setArea(SArea(6, 1, 1, 1));
	
    Elevator = Engine->addElevator();
    Elevator->setArea(SArea(6, 2.5f, 1, 1));
	Elevator->Style = ELT_VERTICAL;
	
    Elevator = Engine->addElevator();
    Elevator->setArea(SArea(10, 0.5f, 1, 1));
	Elevator->Style = ELT_CIRCULAR;

	CCollisionActor * Derp = Engine->addActor();
	Derp->setArea(SArea(4, 0, 1, 1));


	// Time-independent movement variables
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
				if (event.key.keysym.sym == SDLK_ESCAPE)
					exit(0);
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

		if (IsKeyDown[SDLK_d] && IsKeyDown[SDLK_a])
		{
			Player->setAction(CCollisionActor::EActionType::None);
		}
		else if (IsKeyDown[SDLK_d])
		{
			Player->setAction(CCollisionActor::EActionType::MoveRight);
		}
		else if (IsKeyDown[SDLK_a])
		{
			Player->setAction(CCollisionActor::EActionType::MoveLeft);
		}
		else
		{
			Player->setAction(CCollisionActor::EActionType::None);
		}


		Player->setJumping(IsKeyDown[SDLK_SPACE]);

		Derp->setJumping(true);

		Engine->updateAll(Delta);

		Draw();

		for (CCollisionEngine::ObjectList::const_iterator it = Engine->getObjects().begin(); it != Engine->getObjects().end(); ++ it)
			(* it)->draw();

		for (CCollisionEngine::ActorList::const_iterator it = Engine->getActors().begin(); it != Engine->getActors().end(); ++ it)
			(* it)->draw();

		SDL_GL_SwapBuffers();
	}

   	return 0;
}

