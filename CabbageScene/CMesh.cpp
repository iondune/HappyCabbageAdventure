#include "CMesh.h"

CMesh::CMesh()
    : PositionBuffer(0), ColorBuffer(0), NormalBuffer(0), TexCoordBuffer(0), NormalLineBuffer(0), NormalColorBuffer(0), IndexBuffer(0), NormalIndexBuffer(0)
{}

CMesh::~CMesh()
{}

void CMesh::centerMeshByAverage(SVector3 const & CenterLocation)
{
    SVector3 VertexSum;
    for (std::vector<SVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        VertexSum += it->Position;

    VertexSum /= (float) Vertices.size();
    SVector3 VertexOffset = CenterLocation - VertexSum;
    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Position += VertexOffset;
}

void CMesh::centerMeshByExtents(SVector3 const & CenterLocation)
{
    if (Vertices.size() < 2)
        return;

    SVector3 Min, Max;
    {
        std::vector<SVertex>::const_iterator it = Vertices.begin();
        Min = it->Position;
        Max = it->Position;
        for (; it != Vertices.end(); ++ it)
        {
        if (Min.X > it->Position.X)
            Min.X = it->Position.X;
        if (Min.Y > it->Position.Y)
            Min.Y = it->Position.Y;
        if (Min.Z > it->Position.Z)
            Min.Z = it->Position.Z;

        if (Max.X < it->Position.X)
            Max.X = it->Position.X;
        if (Max.Y < it->Position.Y)
            Max.Y = it->Position.Y;
        if (Max.Z < it->Position.Z)
            Max.Z = it->Position.Z;
        }
    }

    SVector3 Center = (Max + Min) / 2;

    SVector3 VertexOffset = CenterLocation - Center;
    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Position += VertexOffset;
}

void CMesh::resizeMesh(SVector3 const & Scale)
{
    if (Vertices.size() < 2)
        return;

    SVector3 Extent = getExtent();
    SVector3 Resize = Scale / std::max(Extent.X, std::max(Extent.Y, Extent.Z));
    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Position *= Resize;
}

SVector3 const CMesh::getExtent() const
{
    if (Vertices.size() < 2)
        return SVector3();

    SVector3 Min, Max;
    {
        std::vector<SVertex>::const_iterator it = Vertices.begin();
        Min = it->Position;
        Max = it->Position;
        for (; it != Vertices.end(); ++ it)
        {
            if (Min.X > it->Position.X)
                Min.X = it->Position.X;
            if (Min.Y > it->Position.Y)
                Min.Y = it->Position.Y;
            if (Min.Z > it->Position.Z)
                Min.Z = it->Position.Z;

            if (Max.X < it->Position.X)
                Max.X = it->Position.X;
            if (Max.Y < it->Position.Y)
                Max.Y = it->Position.Y;
            if (Max.Z < it->Position.Z)
                Max.Z = it->Position.Z;
        }
    }

    return (Max - Min);
}

void CMesh::calculateNormalsPerFace()
{
    for (std::vector<STriangle>::iterator it = Triangles.begin(); it != Triangles.end(); ++ it)
    {
        it->Normal = (Vertices[it->Indices[1]].Position - Vertices[it->Indices[0]].Position).
         crossProduct(Vertices[it->Indices[2]].Position - Vertices[it->Indices[0]].Position);
        Vertices[it->Indices[0]].Normal = Vertices[it->Indices[1]].Normal = Vertices[it->Indices[2]].Normal = it->Normal;
    }

    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Normal.normalize();
}

void CMesh::calculateNormalsPerVertex()
{
    calculateNormalsPerFace();

    for (std::vector<STriangle>::iterator it = Triangles.begin(); it != Triangles.end(); ++ it)
    {
        for (int i = 0; i < 3; ++ i)
        {
            Vertices[it->Indices[i]].Normal += it->Normal;
        }
    }

    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Normal.normalize();
}

CBufferObject<float> * CMesh::makePositionBuffer()
{
    if (PositionBuffer)
        return PositionBuffer;

    PositionBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 3; ++ j)
            PositionBuffer->push_back(Vertices[i].Position[j]);
    }

    return PositionBuffer;
}

CBufferObject<float> * CMesh::makeColorBuffer()
{
    if (ColorBuffer)
        return ColorBuffer;

    ColorBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 3; ++ j)
            ColorBuffer->push_back(Vertices[i].Color[j]);
    }

    return ColorBuffer;
}

CBufferObject<float> * CMesh::makeNormalBuffer()
{
    if (NormalBuffer)
        return NormalBuffer;

    NormalBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 3; ++ j)
            NormalBuffer->push_back(Vertices[i].Normal[j]);
    }

    return NormalBuffer;
}

CBufferObject<float> * CMesh::makeTexCoordBuffer()
{
    if (TexCoordBuffer)
        return TexCoordBuffer;

    TexCoordBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 2; ++ j)
            TexCoordBuffer->push_back(Vertices[i].TextureCoordinates[j]);
    }

    return TexCoordBuffer;
}

CBufferObject<unsigned short> * CMesh::makeIndexBuffer()
{
    if (IndexBuffer)
        return IndexBuffer;

    IndexBuffer = new CBufferObject<unsigned short>();

    for(unsigned int i = 0; i < Triangles.size(); ++ i)
        for (unsigned int j = 0; j < 3; ++ j)
            IndexBuffer->push_back(Triangles[i].Indices[j]);

    IndexBuffer->setIsIndexBuffer(true);

    return IndexBuffer;
}

CBufferObject<float> * CMesh::makeNormalLineBuffer()
{
    if (NormalLineBuffer)
        return NormalLineBuffer;

    NormalLineBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 3; ++ j)
            NormalLineBuffer->push_back(Vertices[i].Position[j]);
        for (unsigned int j = 0; j < 3; ++ j)
            NormalLineBuffer->push_back(Vertices[i].Position[j] + Vertices[i].Normal[j]*0.05f);
    }

    return NormalLineBuffer;
}

CBufferObject<float> * CMesh::makeNormalColorBuffer()
{
    if (NormalColorBuffer)
        return NormalColorBuffer;

    NormalColorBuffer = new CBufferObject<float>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 3; ++ j)
            NormalColorBuffer->push_back(Vertices[i].Color[j]);

        for (unsigned int j = 0; j < 3; ++ j)
            NormalColorBuffer->push_back(Vertices[i].Color[j]);
    }

    return NormalColorBuffer;
}

CBufferObject<unsigned short> * CMesh::makeNormalIndexBuffer()
{
    if (NormalIndexBuffer)
        return NormalIndexBuffer;

    NormalIndexBuffer = new CBufferObject<unsigned short>();

    for (unsigned int i = 0; i < Vertices.size(); ++ i)
    {
        for (unsigned int j = 0; j < 2; ++ j)
            NormalIndexBuffer->push_back(i*2 + j);
    }

    NormalIndexBuffer->setIsIndexBuffer(true);

    return NormalIndexBuffer;
}

SBoundingBox3 const CMesh::getBoundingBox() const
{
    if (! Vertices.size())
        return SBoundingBox3();

    SBoundingBox3 Box(Vertices[0].Position);

    for (int i = 1; i != Vertices.size(); ++ i)
        Box.addInternalPoint(Vertices[i].Position);

    return Box;
}

void CMesh::linearizeIndices()
{
    std::vector<SVertex> newVertices;
    std::vector<STriangle> newTriangles;

    for (std::vector<STriangle>::iterator it = Triangles.begin(); it != Triangles.end(); ++ it)
    {
        for (int i = 0; i < 3; ++ i)
            newVertices.push_back(Vertices[it->Indices[i]]);
    }

    for (unsigned int i = 0; i < newVertices.size()/3; ++ i)
    {
        STriangle tri;
        tri.Indices[0] = i*3;
        tri.Indices[1] = i*3 + 1;
        tri.Indices[2] = i*3 + 2;
        newTriangles.push_back(tri);
    }

    Vertices = newVertices;
    Triangles = newTriangles;
}
