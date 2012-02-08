#ifndef _CABBAGE_SCENE_CMESH_H_INCLUDED_
#define _CABBAGE_SCENE_CMESH_H_INCLUDED_

#include "../CabbageCore/SBoundingBox3.h"

#include "SVertex.h"
#include "CBufferObject.h"

#include <vector>

class CMesh
{

    CBufferObject<float> * PositionBuffer, * ColorBuffer, * NormalBuffer, * TexCoordBuffer, * NormalLineBuffer, * NormalColorBuffer;
    CBufferObject<unsigned short> * IndexBuffer, * NormalIndexBuffer;

public:

    struct STriangle
    {
        unsigned int Indices[3];
        SVector3 Normal;
    };

    std::vector<SVertex> Vertices;
    std::vector<STriangle> Triangles;

    CMesh();
    ~CMesh();

    void centerMeshByAverage(SVector3 const & CenterLocation);
    void centerMeshByExtents(SVector3 const & CenterLocation);

    void resizeMesh(SVector3 const & Scale);

    SVector3 const getExtent() const;

    void calculateNormalsPerFace();
    void calculateNormalsPerVertex();

    CBufferObject<float> * makePositionBuffer();
    CBufferObject<float> * makeColorBuffer();
    CBufferObject<float> * makeNormalBuffer();
    CBufferObject<float> * makeTexCoordBuffer();
    CBufferObject<unsigned short> * makeIndexBuffer();

    CBufferObject<float> * makeNormalLineBuffer();
    CBufferObject<float> * makeNormalColorBuffer();
    CBufferObject<unsigned short> * makeNormalIndexBuffer();

    SBoundingBox3 const getBoundingBox() const;

    void linearizeIndices();

};

#endif
