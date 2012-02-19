#include "CMesh.h"

#include <limits>

CMesh::CMesh()
{}

CMesh::~CMesh()
{}

unsigned int const CMesh::getVertexCount() const
{
    unsigned int Count = 0;
    for (std::vector<SMeshBuffer *>::const_iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
        Count += (* bit)->Vertices.size();
    return Count;
}

void CMesh::centerMeshByAverage(SVector3 const & CenterLocation)
{
    SVector3 VertexSum;
    for (std::vector<SMeshBuffer *>::const_iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
        for (std::vector<SVertex>::const_iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
            VertexSum += it->Position;

    VertexSum /= (float) getVertexCount();
    SVector3 VertexOffset = CenterLocation - VertexSum;
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
            it->Position += VertexOffset;
}

void CMesh::centerMeshByExtents(SVector3 const & CenterLocation)
{
    SVector3 Min(std::numeric_limits<float>::max()), Max(-std::numeric_limits<float>::max());
    {
        for (std::vector<SMeshBuffer *>::const_iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
        for (std::vector<SVertex>::const_iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
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
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        it->Position += VertexOffset;
}

void CMesh::resizeMesh(SVector3 const & Scale)
{
    SVector3 Extent = getExtent();
    SVector3 Resize = Scale / std::max(Extent.X, std::max(Extent.Y, Extent.Z));
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        it->Position *= Resize;
}

SVector3 const CMesh::getExtent() const
{
    SVector3 Min(std::numeric_limits<float>::max()), Max(-std::numeric_limits<float>::max());
    {
        for (std::vector<SMeshBuffer *>::const_iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
        for (std::vector<SVertex>::const_iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
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
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
	{
		for (std::vector<STriangle>::iterator it = (* bit)->Triangles.begin(); it != (* bit)->Triangles.end(); ++ it)
		{
			it->Normal = ((* bit)->Vertices[it->Indices[1]].Position - (* bit)->Vertices[it->Indices[0]].Position).
				crossProduct((* bit)->Vertices[it->Indices[2]].Position - (* bit)->Vertices[it->Indices[0]].Position);
			(* bit)->Vertices[it->Indices[0]].Normal = (* bit)->Vertices[it->Indices[1]].Normal = (* bit)->Vertices[it->Indices[2]].Normal = it->Normal;
		}
	}

    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
		for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
			it->Normal.normalize();
}

void CMesh::calculateNormalsPerVertex()
{
    calculateNormalsPerFace();

    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
	{
		for (std::vector<STriangle>::iterator it = (* bit)->Triangles.begin(); it != (* bit)->Triangles.end(); ++ it)
		{
			for (int i = 0; i < 3; ++ i)
			{
				(* bit)->Vertices[it->Indices[i]].Normal += it->Normal;
			}
		}
	}

    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
		for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
			it->Normal.normalize();
}

void CMesh::updateBuffers()
{
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * PositionBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                PositionBuffer->push_back(it->Position[j]);
        }

        PositionBuffers.push_back(PositionBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * ColorBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                ColorBuffer->push_back(it->Color[j]);
        }

        ColorBuffers.push_back(ColorBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * NormalBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                NormalBuffer->push_back(it->Normal[j]);
        }

        NormalBuffers.push_back(NormalBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * TexCoordBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 2; ++ j)
                TexCoordBuffer->push_back(it->TextureCoordinates[j]);
        }

        TexCoordBuffers.push_back(TexCoordBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<unsigned short> * IndexBuffer = new CBufferObject<unsigned short>();

        for (std::vector<STriangle>::iterator it = (* bit)->Triangles.begin(); it != (* bit)->Triangles.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                IndexBuffer->push_back((* it).Indices[j]);
        }

        IndexBuffer->setIsIndexBuffer(true);
        IndexBuffers.push_back(IndexBuffer);
    }
    

	static float const LengthFactor = 0.05f;

    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * NormalLineBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                NormalLineBuffer->push_back(it->Position[j]);
            for (unsigned int j = 0; j < 3; ++ j)
                NormalLineBuffer->push_back(it->Position[j] + it->Normal[j]*LengthFactor);
        }

        NormalLineBuffers.push_back(NormalLineBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<float> * NormalColorBuffer = new CBufferObject<float>();

        for (std::vector<SVertex>::iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        {
            for (unsigned int j = 0; j < 3; ++ j)
                NormalColorBuffer->push_back(it->Color[j]);

            for (unsigned int j = 0; j < 3; ++ j)
                NormalColorBuffer->push_back(it->Color[j]);
        }

        NormalColorBuffers.push_back(NormalColorBuffer);
    }


    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        CBufferObject<unsigned short> * NormalIndexBuffer = new CBufferObject<unsigned short>();

        for (unsigned int i = 0; i < (* bit)->Vertices.size(); ++ i)
        {
            for (unsigned int j = 0; j < 2; ++ j)
                NormalIndexBuffer->push_back(i*2 + j);
        }

        NormalIndexBuffer->setIsIndexBuffer(true);

        NormalIndexBuffers.push_back(NormalIndexBuffer);
    }
}

SBoundingBox3 const CMesh::getBoundingBox() const
{
    SBoundingBox3 Box(SVector3(std::numeric_limits<float>().max()), SVector3(-std::numeric_limits<float>().max()));

    for (std::vector<SMeshBuffer *>::const_iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    for (std::vector<SVertex>::const_iterator it = (* bit)->Vertices.begin(); it != (* bit)->Vertices.end(); ++ it)
        Box.addInternalPoint(it->Position);

    return Box;
}

void CMesh::linearizeIndices()
{
    for (std::vector<SMeshBuffer *>::iterator bit = MeshBuffers.begin(); bit != MeshBuffers.end(); ++ bit)
    {
        std::vector<SVertex> newVertices;
        std::vector<STriangle> newTriangles;
        
        for (std::vector<STriangle>::iterator it = (* bit)->Triangles.begin(); it != (* bit)->Triangles.end(); ++ it)
        {
            for (int i = 0; i < 3; ++ i)
                newVertices.push_back((* bit)->Vertices[it->Indices[i]]);
        }

        for (unsigned int i = 0; i < newVertices.size()/3; ++ i)
        {
            STriangle tri;
            tri.Indices[0] = i*3;
            tri.Indices[1] = i*3 + 1;
            tri.Indices[2] = i*3 + 2;
            newTriangles.push_back(tri);
        }

        (* bit)->Vertices = newVertices;
        (* bit)->Triangles = newTriangles;
    }
}
