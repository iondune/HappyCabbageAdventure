#include <string>
#include <sstream>
#include "Cimg-1.4.9\CImg.h"
#include "irrxml-1.2\src\irrxml.h"
#include "levelReader.h"

// Camera Coordinates
float x = 0, y = 0, z = 2;
float yr = 0, zr = -1.5708f;
//using namespace cimg_library;

int main()
{
	/*unsigned char *ptr;
	CImg<unsigned char> src("level1.bmp");
	int width = src.width();
	int height = src.height();
	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			ptr = src.data(j,i);
			if ((ptr[0] != 0 && ptr[1] != 0 && ptr[2] != 0) && (ptr[0] != 255 && ptr[1] != 255 && ptr[2] !=255))
				printf("%d , %d, %d, %d \n", ptr[0],ptr[1], ptr[2], ptr[3]);
		}
	}*/
	levReader();

	std::vector<plat> list;
    std::vector<obj> character;

	xmlLevReader(list, character, "level4.xml");

	system("PAUSE");
   	return 0;
}
