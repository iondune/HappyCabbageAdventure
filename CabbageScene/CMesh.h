#ifndef _CABBAGE_SCENE_CMESH_H_INCLUDED_
#define _CABBAGE_SCENE_CMESH_H_INCLUDED_

#include "../CabbageCore/SBoundingBox3.h"

#include "SVertex.h"
#include "CBufferObject.h"

#include <vector>

class CMesh
{

public:

    struct STriangle
    {
        unsigned int Indices[3];
        SVector3 Normal;
    };

    struct SMeshBuffer
    {
        std::vector<SVertex> Vertices;
        std::vector<STriangle> Triangles;

        SColor DiffuseColor;
    };

    std::vector<SMeshBuffer *> MeshBuffers;

	std::vector<CBufferObject<float> *> PositionBuffers, ColorBuffers, NormalBuffers, TexCoordBuffers, NormalLineBuffers, NormalColorBuffers;
    std::vector<CBufferObject<unsigned short> *> IndexBuffers, NormalIndexBuffers;

    CMesh();
    ~CMesh();

    unsigned int const getVertexCount() const;

    void centerMeshByAverage(SVector3 const & CenterLocation);
    void centerMeshByExtents(SVector3 const & CenterLocation);

    void resizeMesh(SVector3 const & Scale);

    SVector3 const getExtent() const;

    void calculateNormalsPerFace();
    void calculateNormalsPerVertex();

    void updateBuffers();

    SBoundingBox3 const getBoundingBox() const;

    void linearizeIndices();

};

#endif
