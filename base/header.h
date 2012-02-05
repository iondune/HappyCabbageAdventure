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
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#include <GL/glew.h>
#endif

#ifdef __unix__
#include "SDL/SDL_opengl.h"
#endif

//Local Include Section
#include "../CabbageCollider/CEngine.h"
#include "FreeType.h"
#include "CPlayerView.h"

#include "Materials.h"
#include "texture.h"
#include "sound.h"

//Global Defines
#define NUM_TREES 10

#include "CGameplayManager.h"
