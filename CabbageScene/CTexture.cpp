#include "CTexture.h"

#ifdef __unix__
#include <GL/glu.h>
#endif

CTexture::CTexture(CImage * image)
    : Width(image->getWidth()), Height(image->getHeight())
{
    static int TextureCounter = 0;

    TextureHandle = ++ TextureCounter;

    glBindTexture(GL_TEXTURE_2D, TextureHandle); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->getImageData());

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, image->getImageData());
}

GLuint const CTexture::getTextureHandle() const
{
    return TextureHandle;
}

int const CTexture::getWidth() const
{
    return Width;
}

int const CTexture::getHeight() const
{
    return Height;
}
