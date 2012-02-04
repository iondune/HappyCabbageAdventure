#include "CMeshLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <sys/stat.h>

#pragma warning(disable: 4996)

static long filelength(int f)
{
    struct stat buf;

    fstat(f, &buf);

    return(buf.st_size);
}

// TODO: Rewrite this horrendous pile of C nonsense
CMesh * const CMeshLoader::load3dsMesh(std::string const & fileName)
{
    int i; //Index variable

    FILE *l_file; //File pointer

    unsigned short l_chunk_id; //Chunk identifier
    unsigned int l_chunk_lenght; //Chunk lenght

    unsigned char l_char; //Char variable
    unsigned short l_qty; //Number of elements in each chunk

    unsigned short l_face_flags; //Flag that stores some face information

    if ((l_file=fopen (fileName.c_str(), "rb"))== NULL) return 0; //Open the file

    CMesh * Mesh = new CMesh();

	while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
	//while(!EOF)
	{
		//getch(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		//printf("ChunkID: %x\n",l_chunk_id); 
		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		//printf("ChunkLenght: %x\n",l_chunk_lenght);

		switch (l_chunk_id)
        {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
			break;    

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;
			
			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000: 
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    //p_object->name[i]=l_char; // Throw out name - why do we care? we don't! bwahahahaha
					i++;
                }while(l_char != '\0' && i<20);
			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
			break;
			
			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices) 
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110: 
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				Mesh->Vertices.resize(l_qty);
                //printf("Number of vertices: %d\n",l_qty);
                for (i=0; i<l_qty; i++)
                {
					fread (& Mesh->Vertices[i].Position.X, sizeof(float), 1, l_file);
 					//printf("Vertices list x: %f\n",Mesh->Vertices[i].Position.X);
                    fread (& Mesh->Vertices[i].Position.Y, sizeof(float), 1, l_file);
 					//printf("Vertices list y: %f\n",Mesh->Vertices[i].Position.Y);
					fread (& Mesh->Vertices[i].Position.Z, sizeof(float), 1, l_file);
 					//printf("Vertices list z: %f\n",Mesh->Vertices[i].Position.Z);
				}
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons) 
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
			case 0x4120:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                Mesh->Triangles.resize(l_qty);
                //printf("Number of polygons: %d\n",l_qty); 
                for (i=0; i<l_qty; i++)
                {
                    Mesh->Triangles[i].Indices[0] = 0;
                    Mesh->Triangles[i].Indices[1] = 0;
                    Mesh->Triangles[i].Indices[2] = 0;
					fread (& Mesh->Triangles[i].Indices[0], sizeof (unsigned short), 1, l_file);
					//printf("Polygon point a: %d\n", Mesh->Triangles[i].Indices[0]);
					fread (& Mesh->Triangles[i].Indices[1], sizeof (unsigned short), 1, l_file);
					//printf("Polygon point b: %d\n", Mesh->Triangles[i].Indices[1]);
					fread (& Mesh->Triangles[i].Indices[2], sizeof (unsigned short), 1, l_file);
					//printf("Polygon point c: %d\n", Mesh->Triangles[i].Indices[2]);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					//printf("Face flags: %x\n",l_face_flags);
				}
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points) 
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (& Mesh->Vertices[i].TextureCoordinates[0], sizeof (float), 1, l_file);
					//printf("Mapping list u: %f\n", Mesh->Vertices[i].TextureCoordinates[0]);
                    fread (& Mesh->Vertices[i].TextureCoordinates[1], sizeof (float), 1, l_file);
					//printf("Mapping list v: %f\n", Mesh->Vertices[i].TextureCoordinates[1]);
				}
                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        } 
	}
	fclose (l_file); // Closes the file stream
	return Mesh; // Returns ok
}

CMesh * const CMeshLoader::loadAsciiMesh(std::string const & fileName)
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
            Triangle.Indices[0] = Vertex1 - 1;
			Triangle.Indices[1] = Vertex2 - 1;
			Triangle.Indices[2] = Vertex3 - 1;

			size_t Location;
			if ((Location = ReadString.find("{")) != std::string::npos) // there is a color
			{
				Location = ReadString.find("rgb=(");
				Location += 5; // rgb=( is 5 characters

				ReadString = ReadString.substr(Location);
				std::stringstream Stream(ReadString);
				float Color;
				Stream >> Color;
				//Triangle.Color.Red = Color;
				Stream >> Color;
				//Triangle.Color.Green = Color;
				Stream >> Color;
				//Triangle.Color.Blue = Color;
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

CMesh * const CMeshLoader::createCubeMesh()
{
    CMesh * Mesh = new CMesh();
    Mesh->Vertices.resize(8);
    Mesh->Vertices[0].Position = SVector3(-0.5, -0.5, -0.5);
    Mesh->Vertices[1].Position = SVector3(-0.5,  0.5, -0.5);
    Mesh->Vertices[2].Position = SVector3( 0.5,  0.5, -0.5);
    Mesh->Vertices[3].Position = SVector3( 0.5, -0.5, -0.5);
    Mesh->Vertices[4].Position = SVector3(-0.5, -0.5,  0.5);
    Mesh->Vertices[5].Position = SVector3(-0.5,  0.5,  0.5);
    Mesh->Vertices[6].Position = SVector3( 0.5,  0.5,  0.5);
    Mesh->Vertices[7].Position = SVector3( 0.5, -0.5,  0.5);

    Mesh->Triangles.resize(12);
    Mesh->Triangles[0].Indices[0] = 0;
    Mesh->Triangles[0].Indices[1] = 1;
    Mesh->Triangles[0].Indices[2] = 2;

    Mesh->Triangles[1].Indices[0] = 0;
    Mesh->Triangles[1].Indices[1] = 2;
    Mesh->Triangles[1].Indices[2] = 3;

    Mesh->Triangles[2].Indices[0] = 7;
    Mesh->Triangles[2].Indices[1] = 6;
    Mesh->Triangles[2].Indices[2] = 4;

    Mesh->Triangles[3].Indices[0] = 4;
    Mesh->Triangles[3].Indices[1] = 6;
    Mesh->Triangles[3].Indices[2] = 5;

    Mesh->Triangles[4].Indices[0] = 0;
    Mesh->Triangles[4].Indices[1] = 3;
    Mesh->Triangles[4].Indices[2] = 7;

    Mesh->Triangles[5].Indices[0] = 0;
    Mesh->Triangles[5].Indices[1] = 7;
    Mesh->Triangles[5].Indices[2] = 4;

    Mesh->Triangles[6].Indices[0] = 4;
    Mesh->Triangles[6].Indices[1] = 5;
    Mesh->Triangles[6].Indices[2] = 1;

    Mesh->Triangles[7].Indices[0] = 4;
    Mesh->Triangles[7].Indices[1] = 1;
    Mesh->Triangles[7].Indices[2] = 0;

    Mesh->Triangles[8].Indices[0] = 3;
    Mesh->Triangles[8].Indices[1] = 2;
    Mesh->Triangles[8].Indices[2] = 6;

    Mesh->Triangles[9].Indices[0] = 3;
    Mesh->Triangles[9].Indices[1] = 6;
    Mesh->Triangles[9].Indices[2] = 7;

    Mesh->Triangles[10].Indices[0] = 1;
    Mesh->Triangles[10].Indices[1] = 5;
    Mesh->Triangles[10].Indices[2] = 6;

    Mesh->Triangles[11].Indices[0] = 1;
    Mesh->Triangles[11].Indices[1] = 6;
    Mesh->Triangles[11].Indices[2] = 2;

    return Mesh;
}
