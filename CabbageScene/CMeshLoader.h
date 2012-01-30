#ifndef _CABBAGE_SCENE_CMESHLOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CMESHLOADER_H_INCLUDED_

#include "CMesh.h"

#include <string>

class CMeshLoader
{

public:

	static CMesh * const load3dsMesh(std::string const & fileName);

    static CMesh * const createCubeMesh();

};

#endif
