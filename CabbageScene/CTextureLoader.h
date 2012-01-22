#ifndef _CABBAGE_SCENE_CTEXTURELOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CTEXTURELOADER_H_INCLUDED_

#include "CTexture.h"

#include <string>

class CTextureLoader
{

public:

	static CTexture * const loadTexture(std::string const & fileName);

};

#endif
