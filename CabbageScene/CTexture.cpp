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
	: TextureHandle(0), Size()
{
	if (Image)
	{
		Size.Width = Image->getWidth();
		Size.Height = Image->getHeight();

		glGenTextures(1, & TextureHandle);
		glBindTexture(GL_TEXTURE_2D, TextureHandle);

		Flags.apply();

		glTexImage2D(GL_TEXTURE_2D, 0, Image->hasAlpha() ? GL_RGBA8 : GL_RGB8, Size.Width, Size.Height, 0, Image->hasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, Image->getImageData());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cerr << "Aborting texture creation, image not loaded!" << std::endl;
	}
}

CTexture::CTexture(int const width, int const height, bool const Alpha, STextureCreationFlags const Flags)
	: TextureHandle(0), Size(width, height)
{
	glGenTextures(1, & TextureHandle);
	glBindTexture(GL_TEXTURE_2D, TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, Alpha ? GL_RGBA8 : GL_RGB8, Size.Width, Size.Height, 0, Alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Flags.apply();
}

CTexture::CTexture(SPosition2 const & size, bool const Alpha, STextureCreationFlags const Flags)
	: TextureHandle(0), Size(size)
{
	glGenTextures(1, & TextureHandle);
	glBindTexture(GL_TEXTURE_2D, TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, Alpha ? GL_RGBA8 : GL_RGB8, Size.Width, Size.Height, 0, Alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Flags.apply();
}

CTexture::CTexture(GLuint const textureHandle)
	: TextureHandle(textureHandle)
{
	GLint Width, Height;

	glBindTexture(GL_TEXTURE_2D, TextureHandle);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, & Width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, & Height);

	Size.Width = Width;
	Size.Height = Height;
}

CTexture::~CTexture()
{
	glDeleteTextures(1, & TextureHandle);
}

GLuint const CTexture::getTextureHandle() const
{
    return TextureHandle;
}

SSize2 const & CTexture::getSize() const
{
	return Size;
}

int const CTexture::getWidth() const
{
    return Size.Width;
}

int const CTexture::getHeight() const
{
    return Size.Height;
}

bool const CTexture::isValid() const
{
	return TextureHandle != 0;
}
