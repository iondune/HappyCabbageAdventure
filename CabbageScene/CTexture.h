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
#include "CImage.h"

class CTexture
{

	friend class CTextureLoader;
	friend class CRenderable;

	GLuint TextureHandle;
	int Width;
	int Height;

public:

    CTexture(CImage * image);

	int const getWidth();
	int const getHeight();

};

#endif
