#ifndef _CABBAGE_SCENE_CIMAGE_H_INCLUDED_
#define _CABBAGE_SCENE_CIMAGE_H_INCLUDED_

#include <string>

#ifdef __unix__
#include<GL/gl.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#endif

#include "../CabbageCore/SVector2.h"

class CImage
{

    friend class CImageLoader;

    char * ImageData;
    int Width;
    int Height;

    CImage(char * imageData, int width, int height);

public:

    ~CImage();

    int const getWidth();
    int const getHeight();
    char const * const getImageData();

};

#endif
