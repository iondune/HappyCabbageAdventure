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

    GLuint TextureHandle;
    int Width;
    int Height;

public:

    CTexture(CImage * image);

    GLuint const getTextureHandle() const;

    int const getWidth() const;
    int const getHeight() const;

};

#endif
