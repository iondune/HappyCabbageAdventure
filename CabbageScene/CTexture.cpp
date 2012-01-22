#include "CTexture.h"

CTexture::CTexture(GLuint const textureHandle, int const width, int const height)
	: TextureHandle(textureHandle), Width(width), Height(height)
{}

int const CTexture::getWidth()
{
	return Width;
}

int const CTexture::getHeight()
{
	return Height;
}
