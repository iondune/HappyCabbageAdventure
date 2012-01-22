#ifndef _CABBAGE_SCENE_CTEXTURE_H_INCLUDED_
#define _CABBAGE_SCENE_CTEXTURE_H_INCLUDED_

#include <string>

#ifdef __unix__
#include<GL/gl.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#endif

#include "../CabbageCore/SVector2.h"

class CTexture
{

	friend class CTextureLoader;
	friend class CRenderable;

	GLuint const TextureHandle;
	int Width;
	int Height;

	CTexture(GLuint const textureHandle, int width, int height);

public:

	int const getWidth();
	int const getHeight();

};

#endif
