#include "CTexture.h"

#ifdef __unix__
#include <GL/glu.h>
#endif

#include <iostream>

STextureCreationFlags::STextureCreationFlags()
{
	MipMaps = true;
	Wrap = GL_REPEAT;
	Filter = GL_LINEAR;
}

void STextureCreationFlags::apply() const
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap);
	if (MipMaps)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
			Filter == GL_LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
	}
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);
}

CTexture::CTexture(CImage * Image, STextureCreationFlags const Flags)
	: TextureHandle(0), Width(0), Height(0)
{
	if (Image)
	{
		Width = Image->getWidth();
		Height = Image->getHeight();

		glGenTextures(1, & TextureHandle);
		glBindTexture(GL_TEXTURE_2D, TextureHandle);

		Flags.apply();

		glTexImage2D(GL_TEXTURE_2D, 0, Image->hasAlpha() ? GL_RGBA8 : GL_RGB8, Width, Height, 0, Image->hasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, Image->getImageData());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cerr << "Aborting texture creation, image not loaded!" << std::endl;
	}
}

CTexture::CTexture(int const width, int const height, bool const Alpha, STextureCreationFlags const Flags)
	: TextureHandle(0), Width(width), Height(height)
{
	glGenTextures(1, & TextureHandle);
	glBindTexture(GL_TEXTURE_2D, TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, Alpha ? GL_RGBA8 : GL_RGB8, Width, Height, 0, Alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Flags.apply();
}

CTexture::CTexture(GLuint const textureHandle)
{
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

bool const CTexture::isValid() const
{
	return TextureHandle != 0;
}
