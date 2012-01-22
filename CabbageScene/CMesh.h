#ifndef _CABBAGE_SCENE_CMESH_H_INCLUDED_
#define _CABBAGE_SCENE_CMESH_H_INCLUDED_

#include "SVertex.h"

#include <vector>

class CMesh
{

public:

	struct STriangle
    {
		unsigned int Indices[3];
    };

    std::vector<SVertex> Vertices;
    std::vector<STriangle> Triangles;

    CMesh();
    ~CMesh();

	void centerMeshByAverage(SVector3 const & CenterLocation);
	void centerMeshByExtents(SVector3 const & CenterLocation);

	void resizeMesh(SVector3 const & Scale);

	SVector3 const getExtent() const;

};

#endif
