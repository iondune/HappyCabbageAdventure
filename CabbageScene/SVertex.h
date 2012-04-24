#ifndef _CABBAGE_SCENE_SVERTEX_H_INCLUDED_
#define _CABBAGE_SCENE_SVERTEX_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SVector2.h"
#include "../CabbageCore/SColor.h"

class SVertex
{

public:

	SVector3f Position;
    SVector3f Normal;
	SVector2 TextureCoordinates;

	SColor Color;

};

#endif
