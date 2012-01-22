#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

// Utility classes for loading shaders/meshes
#include "../CabbageScene/CShader.h"
#include "../CabbageScene/CRenderable.h"
#include "../CabbageScene/CMeshLoader.h"
#include "../CabbageScene/CTextureLoader.h"

// Portable version of system("PAUSE")
void waitForUser() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

/********************
 * Global Varaibles *
 ********************/

// Shader and Mesh utility classes
CShader * Shader;
CMesh * Mesh;
CRenderable * Renderable;
CTexture * Texture;

// Information about mesh
SVector3 Translation, Rotation, Scale(1);
int TriangleCount;

// Window information
int WindowWidth = 400, WindowHeight = 400;

// Time-independant movement variables
int Time0, Time1;
float ScaleTimer;


/***************************
 * GLUT Callback Functions *
 ***************************/

// OpenGL initialization
void Initialize()
{
	glClearColor(0.6f, 0.3f, 0.9f, 1.0f);
 	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Start keeping track of time
	Time0 = SDL_GetTicks();
}

// Manages time independant movement and draws the VBO
void Display()
{
	// Determine time since last draw
	Time1 = SDL_GetTicks();
	float Delta = (float) (Time1 - Time0) / 1000.f;
	Time0 = Time1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		0, 0, 2, 
		0, 0, 0, 
		0, 1, 0);

	// Animates the loaded model by modulating it's size
	static float const ScaleSpeed = 1.f;
	static float const ScaleThreshold = 0.4f;
	ScaleTimer += Delta * ScaleSpeed;
	Scale = SVector3(1) + ScaleThreshold * cos(ScaleTimer);

	// ...and by spinning it around
	static float const RotationSpeed = 50.f;
	Rotation.X += RotationSpeed*Delta;
	Rotation.Y += RotationSpeed*Delta*2;

	Renderable->setTranslation(Translation);
	Renderable->setScale(Scale);
	Renderable->setRotation(Rotation);

	Renderable->draw(* Shader, * Texture);

	SDL_GL_SwapBuffers();
}

void Reshape(int width, int height)								
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	WindowWidth = width;
	WindowHeight = height;

	// Set camera projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float AspectRatio = (float)WindowWidth / (float)WindowHeight;
	gluPerspective(60.0, AspectRatio, 0.01, 100.0);
}


int main(int argc, char * argv[])
{
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

	Reshape(WindowWidth, WindowHeight);

	// On Windows, use glew to load shader extensions (OpenGL 2.0 +)
#ifdef _WIN32
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error initializing glew! " << glewGetErrorString(err) << std::endl;
		waitForUser();
		return 1;
	}
#endif

   	Initialize();

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
	Shader->loadAttribute("aTexCoord");
	Shader->loadUniform("uTexColor");

	// Attempt to load mesh
	CMesh * Mesh = CMeshLoader::load3dsMesh("spaceship.3ds");
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

	Texture = CTextureLoader::loadTexture("spaceshiptexture.bmp");
	if (! Texture)
	{
		std::cerr << "Unable to load necessary texture." << std::endl;
		waitForUser();
		return 1;
	}

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	Renderable = new CRenderable(* Mesh);

	SDL_Event event;

    while (1)
	{
        /* process pending events */
        while( SDL_PollEvent( &event ) )
		{
            switch( event.type )
			{

            case SDL_QUIT:
                exit (0);
                break;

            }
        }

    /* update the screen */    
        Display();

    /* Wait 50ms to avoid using up all the CPU time */
        SDL_Delay( 50 );
    }
	
	// Return 0, not that this will ever be called. Damn you, GLUT!
	return 0;
}
