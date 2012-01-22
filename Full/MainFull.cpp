#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")

#include <GL\glew.h>
#include <GL\glut.h>
#endif

// Utility classes for loading shaders/meshes
#include "CShader.h"
#include "CMeshLoader.h"


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

// Handles for VBOs
GLuint PositionBufferHandle, ColorBufferHandle;

// Information about mesh
SVector3 Translation, Rotation, Scale(1);
int TriangleCount;

// Window information
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Time-independant movement variables
int Time0, Time1;
float ScaleTimer;

SDL_Event event;

/***************************
 * GLUT Callback Functions *
 ***************************/

// OpenGL initialization
void Initialize()
{
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
		std::cerr << "Error initializing SDL! " << std::endl;
  }

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  //Create Window
  if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_OPENGL | SDL_GL_DOUBLEBUFFER ) == NULL )
    {
		std::cerr << "Error setting SDL video mode! " << std::endl;
    }

	glClearColor(0.6f, 0.3f, 0.9f, 1.0f);
 	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glViewport( 0, 0, (GLsizei)(SCREEN_WIDTH), (GLsizei)(SCREEN_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)(SCREEN_WIDTH)/ (float)(SCREEN_HEIGHT), 0.01, 100.0);

	// Start keeping track of time
	Time0 = (int)SDL_GetTicks();

    SDL_WM_SetCaption( "Happy Cabbage Adventure", NULL );
}

// Manages time independant movement and draws the VBO
void Display()
{
	// Determine time since last draw
	Time1 = (int)SDL_GetTicks();
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

	{
		// Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(* Shader);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);

		glPushMatrix();

		glTranslatef(Translation.X, Translation.Y, Translation.Z);
		glRotatef(Rotation.Z, 0, 0, 1);
		glRotatef(Rotation.Y, 0, 1, 0);
		glRotatef(Rotation.X, 1, 0, 0);
		glScalef(Scale.X, Scale.Y, Scale.Z);

		glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
	}

    SDL_GL_SwapBuffers();

}

//unused for now
void Reshape(int width, int height)								
{
  /*
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	WindowWidth = width;
	WindowHeight = height;

	// Set camera projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float AspectRatio = (float)WindowWidth / (float)WindowHeight;
	gluPerspective(60.0, AspectRatio, 0.01, 100.0);
    */
}


int main(int argc, char * argv[])
{
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

    bool finished = false;
    //Here's the main loop
    while(!finished)
    {
      while(SDL_PollEvent( &event ) )
      {
        if(event.type == SDL_QUIT )
        {
          finished = true;
        }
      }

      Display();
    }

    SDL_Quit();

	return 0;
}
