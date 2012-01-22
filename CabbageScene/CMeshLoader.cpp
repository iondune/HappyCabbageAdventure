#include "CMeshLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <sys/stat.h>

#pragma warning(disable: 4996)

long filelength(int f)
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
