#include "CTextureLoader.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

CTexture * const CTextureLoader::loadTexture(std::string const & fileName)
{
	SDL_Surface * Surface;
	GLenum Format;

	Surface = IMG_Load(fileName.c_str());

	if (! Surface)
		return 0;

	switch (Surface->format->BytesPerPixel)
	{
	case 3:
		Format = GL_RGB;
		break;
	case 4:
		Format = GL_RGBA;
		break;
	default:
		SDL_FreeSurface(Surface);
		return 0;
	}

	GLuint TextureHandle;

	glGenTextures(1, & TextureHandle);
	glBindTexture(GL_TEXTURE_2D, TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, Format, Surface->w, Surface->h, 0, Format, GL_UNSIGNED_BYTE, Surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CTexture * Texture = new CTexture(TextureHandle, Surface->w, Surface->h);

	SDL_FreeSurface(Surface);

	return Texture;
}
