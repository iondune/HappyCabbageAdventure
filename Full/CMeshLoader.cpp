#include "CMeshLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

CMesh * const CMeshLoader::loadASCIIMesh(std::string const & fileName)
{
	std::ifstream File;
	File.open(fileName.c_str());

	if (! File.is_open())
	{
		std::cerr << "Unable to open mesh file: " << fileName << std::endl;
		return 0;
	}

	CMesh * Mesh = new CMesh();

	while (File)
	{
		std::string ReadString;
		std::getline(File, ReadString);

		std::stringstream Stream(ReadString);

		std::string Label;
		Stream >> Label;

		if (Label.find("#") != std::string::npos)
		{
			// Comment, skip
			continue;
		}

		if ("Vertex" == Label)
		{
			int Index;
			Stream >> Index; // We don't care, throw it away

			SVector3 Position;
			Stream >> Position.X;
			Stream >> Position.Y;
			Stream >> Position.Z;

			SVertex Vertex;
			Vertex.Position = Position;

			Mesh->Vertices.push_back(Vertex);
		}
		else if ("Face" == Label)
		{
			int Index;
			Stream >> Index; // We don't care, throw it away

			int Vertex1, Vertex2, Vertex3;
			Stream >> Vertex1;
			Stream >> Vertex2;
			Stream >> Vertex3;

			CMesh::STriangle Triangle;
			Triangle.VertexIndex1 = Vertex1 - 1;
			Triangle.VertexIndex2 = Vertex2 - 1;
			Triangle.VertexIndex3 = Vertex3 - 1;

			size_t Location;
			if ((Location = ReadString.find("{")) != std::string::npos) // there is a color
			{
				Location = ReadString.find("rgb=(");
				Location += 5; // rgb=( is 5 characters

				ReadString = ReadString.substr(Location);
				std::stringstream Stream(ReadString);
				float Color;
				Stream >> Color;
				Triangle.Color.Red = Color;
				Stream >> Color;
				Triangle.Color.Green = Color;
				Stream >> Color;
				Triangle.Color.Blue = Color;
			}

			Mesh->Triangles.push_back(Triangle);
		}
		else if ("" == Label)
		{
			// Just a new line, carry on...
		}
		else if ("Corner" == Label)
		{
			// We're not doing any normal calculations... (oops!)
		}
		else
		{
			std::cerr << "While parsing ASCII mesh: Expected 'Vertex' or 'Face' label, found '" << Label << "'." << std::endl;
		}
	}

	if (! Mesh->Triangles.size() || ! Mesh->Vertices.size())
	{
		delete Mesh;
		return 0;
	}

	return Mesh;
}

void CMeshLoader::createVertexBufferObject(CMesh const & Mesh, int & TriangleCount, GLuint & PositionBufferHandle, GLuint & ColorBufferHandle)
{
    glGenBuffers(1, & PositionBufferHandle);
	glGenBuffers(1, & ColorBufferHandle);

	std::vector<GLfloat> VertexPositions;
	std::vector<GLfloat> Colors;

	for(unsigned int j = 0; j < Mesh.Triangles.size(); j++) 
	{
        SVertex Vertex;


        Vertex = Mesh.Vertices[Mesh.Triangles[j].VertexIndex1];

        VertexPositions.push_back(Vertex.Position.X);
        VertexPositions.push_back(Vertex.Position.Y);
        VertexPositions.push_back(Vertex.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);


		Vertex = Mesh.Vertices[Mesh.Triangles[j].VertexIndex2];

        VertexPositions.push_back(Vertex.Position.X);
        VertexPositions.push_back(Vertex.Position.Y);
        VertexPositions.push_back(Vertex.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);


		Vertex = Mesh.Vertices[Mesh.Triangles[j].VertexIndex3];

        VertexPositions.push_back(Vertex.Position.X);
        VertexPositions.push_back(Vertex.Position.Y);
        VertexPositions.push_back(Vertex.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);
	}

    TriangleCount = Mesh.Triangles.size();

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, VertexPositions.size()*sizeof(GLfloat), & VertexPositions.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, Colors.size()*sizeof(GLfloat), & Colors.front(), GL_STATIC_DRAW);
}