#ifndef __HEADER_BASE_INCLUDE_
#define __HEADER_BASE_INCLUDE_
//System Include Section
#include <iostream>
#include <cmath>
#include <cstdlib>

#include <SDL/SDL.h>
#include <time.h>


#ifdef _WIN32
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageCollider.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "../lib/CLWIB.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#include <GL/glew.h>
#endif

#ifdef __unix__
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#include "sound.h"

//Global Defines
#define NUM_TREES 10

#endif
