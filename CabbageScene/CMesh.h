#ifndef _CABBAGE_SCENE_CMESH_H_INCLUDED_
#define _CABBAGE_SCENE_CMESH_H_INCLUDED_

#include "../CabbageCore/SBoundingBox3.h"

#include "SVertex.h"
#include "CBufferObject.h"

#include <vector>

class CMesh
{

    std::vector<CBufferObject<float> *> PositionBuffers, ColorBuffers, NormalBuffers, TexCoordBuffers, NormalLineBuffers, NormalColorBuffers;
    std::vector<CBufferObject<unsigned short> *> IndexBuffers, NormalIndexBuffers;

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

        SVector3 DiffuseColor;
    };

    std::vector<SMeshBuffer *> MeshBuffers;

    CMesh();
    ~CMesh();

    unsigned int const getVertexCount() const;

    void centerMeshByAverage(SVector3 const & CenterLocation);
    void centerMeshByExtents(SVector3 const & CenterLocation);

    void resizeMesh(SVector3 const & Scale);

    SVector3 const getExtent() const;

    void calculateNormalsPerFace();
    void calculateNormalsPerVertex();

    std::vector<CBufferObject<float> *> makePositionBuffer();
    std::vector<CBufferObject<float> *> makeColorBuffer();
    std::vector<CBufferObject<float> *> makeNormalBuffer();
    std::vector<CBufferObject<float> *> makeTexCoordBuffer();
    std::vector<CBufferObject<unsigned short> *> makeIndexBuffer();

    std::vector<CBufferObject<float> *> makeNormalLineBuffer(float const lengthFactor = 0.05f);
    std::vector<CBufferObject<float> *> makeNormalColorBuffer();
    std::vector<CBufferObject<unsigned short> *> makeNormalIndexBuffer();

    SBoundingBox3 const getBoundingBox() const;

    void linearizeIndices();

};

#endif
