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

class STextureCreationFlags
{

public:

	STextureCreationFlags();

	bool MipMaps;
	GLenum Wrap;
	GLenum Filter;

	void apply() const;

};

class CTexture
{

    GLuint TextureHandle;
    int Width;
    int Height;

public:

    CTexture(CImage * Image, STextureCreationFlags const Flags = STextureCreationFlags());
	CTexture(int const width, int const height, bool const Alpha, STextureCreationFlags const Flags = STextureCreationFlags());
	CTexture(GLuint const textureHandle);

    GLuint const getTextureHandle() const;

    int const getWidth() const;
    int const getHeight() const;

	bool const isValid() const;

};

#endif
