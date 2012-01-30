#ifndef _CABBAGE_SCENE_CIMAGELOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CIMAGELOADER_H_INCLUDED_

#include "CImage.h"

#include <string>

class CImageLoader
{

public:

    static CImage * const loadImage(std::string const & fileName);

};

#endif
