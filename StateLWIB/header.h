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
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageCollider.lib")
#pragma comment(lib, "CabbageFramework.lib")
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

//Local Include Section
#include "../CabbageCollider/CCollisionEngine.h"

#include "Materials.h"
#ifdef _ENABLED_CABBAGE_SOUND_
#include "../CabbageSound/sound.h"
#endif

//Global Defines
#define NUM_TREES 10
#endif
