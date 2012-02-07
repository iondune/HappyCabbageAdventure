#include "levelReader.h"
#include "wmlwriter_src\xmlwriter.h"
#include <sstream>
using namespace cimg_library;

void levReader() {
	CImg<unsigned char> src("test.bmp");
	int width = src.width();
	int height = src.height();
	int tempWidth = 0;
	int tempHeight = 0;
	printf("width is %d, height is %d\n", width, height);
	int count = 0;
	unsigned char *ptr;
	unsigned char *tempPtr;
	plat block;
	obj move;
	std::vector<plat>list;
	std::vector<obj>character;
	int firstx;
	int firsty;
	int flag = 0; // flag for continuation!

	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			ptr = src.data(j,i);
			//printf("%d , %d, %d, %d \n", ptr[0],ptr[1], ptr[2], ptr[3]);
			//Object reader
			if ((ptr[0] == 63 && ptr[1] == 63 && ptr[2] == 63) || (ptr[0] == 237 && ptr[1] == 237 && ptr[2] == 237))
			{ // check if it is a black pixle
				if(character.size() == 0) 
				{
					firstx = j;
					firsty = i;
					printf("first colored pixle is at x %d y %d\n", firstx, firsty);
					tempPtr = src.data(firstx, firsty);
					for (int x = 0; x < width; x++)
					{
						tempPtr = src.data(firstx+x, firsty);
						//printf("%d , %d, %d, %d \n", tempPtr[0],tempPtr[1], tempPtr[2], tempPtr[3]);
						if((tempPtr[0] != 63 || tempPtr[1] !=63 || tempPtr[2] != 63 || tempPtr[3] != 63) ||
							(tempPtr[0] != 237 || tempPtr[1] !=237 || tempPtr[2] != 237 || tempPtr[3] != 237))
							break;
						tempWidth = x;
					}
					tempPtr = src.data(firstx, firsty);
					for (int y = 0; y < height; y++)
					{
						tempPtr = src.data(firstx, firsty+y);
						if((tempPtr[0] != 63 || tempPtr[1] !=63 || tempPtr[2] != 63 || tempPtr[3] != 63) ||
							(tempPtr[0] != 237 || tempPtr[1] !=237 || tempPtr[2] != 237 || tempPtr[3] != 237))
							break;
						tempHeight = y;
					}
					move.height = tempHeight;
					move.width = tempWidth;
					move.topX = firstx;
					move.topY = firsty;
					if (ptr[0] == 63)
						move.main = true;
					if (ptr[0] == 237)
						move.main = false;
					character.push_back(move);
					if(character[character.size()-1].main == true)
						printf("character is main\n");
					else
						printf("character is enemy\n");
					printf("there are %d moveable objects\n", character.size());
					printf("temp height = %d, temp width = %d\n", tempHeight, tempWidth);
					j += tempWidth;
				}
				if (character.size() > 0) 
				{
					//as of right now this code assumes that there will be no platforms on the same y-axis
					// must add a check for y componet later!
					//printf("x is %d, y is %d\n", j, i);
					for(int x = 0; x < (int)character.size(); x++)
					{
						if (character[x].topX == j || ((j > character[x].topX) && (j < (character[x].topX + character[x].width + 2))))
						{	
							if (character[x].topY == i || (i > character[x].topY) && (i < (character[x].topY + character[x].height + 2)))
							{
								j += character[x].width;
								flag = 1;
								//printf("here\n");
								break;
							}
						}
					}
					if(flag == 1)
					{
						flag = 0;
						continue;
					}
					else
					{
						firstx = j;
						firsty = i;
						printf("next black pixle is at x %d y %d\n", firstx, firsty);
						tempPtr = src.data(firstx, firsty);
						for (int x = 0; x < width; x++)
						{
							tempPtr = src.data(firstx+x, firsty);
							//printf("%d , %d, %d, %d \n", tempPtr[0],tempPtr[1], tempPtr[2], tempPtr[3]);
						if((tempPtr[0] != 63 || tempPtr[1] !=63 || tempPtr[2] != 63 || tempPtr[3] != 63) ||
							(tempPtr[0] != 237 || tempPtr[1] !=237 || tempPtr[2] != 237 || tempPtr[3] != 237))
								break;
							tempWidth = x;
						}
						tempPtr = src.data(firstx, firsty);
						for (int y = 0; y < height; y++)
						{
							tempPtr = src.data(firstx, firsty+y);
						if((tempPtr[0] != 63 || tempPtr[1] !=63 || tempPtr[2] != 63 || tempPtr[3] != 63) ||
							(tempPtr[0] != 237 || tempPtr[1] !=237 || tempPtr[2] != 237 || tempPtr[3] != 237))
								break;
							tempHeight = y;
						}
						move.height = tempHeight;
						move.width = tempWidth;
						move.topX = firstx;
						move.topY = firsty;
						if (ptr[0] == 63)
							move.main = true;
						if (ptr[0] == 237)
							move.main = false;
						character.push_back(move);
						if(character[character.size()-1].main == true)
							printf("character is main\n");
						else
							printf("character is enemy\n");
						printf("there are %d moveable obj\n", character.size());
						printf("temp height = %d, temp width = %d\n", tempHeight, tempWidth);
						j += tempWidth;
					}
				}
			}
			//platform reader
			if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 0)
			{ // check if it is a black pixle
				if(list.size() == 0) 
				{
					firstx = j;
					firsty = i;
					printf("first black pixle is at x %d y %d\n", firstx, firsty);
					tempPtr = src.data(firstx, firsty);
					for (int x = 0; x < width; x++)
					{
						tempPtr = src.data(firstx+x, firsty);
						//printf("%d , %d, %d, %d \n", tempPtr[0],tempPtr[1], tempPtr[2], tempPtr[3]);
						if(tempPtr[0] != 0 || tempPtr[1] !=0 || tempPtr[2] != 0 || tempPtr[3] != 0)
							break;
						tempWidth = x;
					}
					tempPtr = src.data(firstx, firsty);
					for (int y = 0; y < height; y++)
					{
						tempPtr = src.data(firstx, firsty+y);
						if(tempPtr[0] != 0 || tempPtr[1] !=0 || tempPtr[2] != 0 || tempPtr[3] != 0)
							break;
						tempHeight = y;
					}
					block.height = tempHeight;
					block.width = tempWidth;
					block.topX = firstx;
					block.topY = firsty;
					list.push_back(block);
					printf("there are %d platforms\n", list.size());
					printf("temp height = %d, temp width = %d\n", tempHeight, tempWidth);
					j += tempWidth;
				}
				if (list.size() > 0) 
				{
					//as of right now this code assumes that there will be no platforms on the same y-axis
					// must add a check for y componet later!
					//printf("x is %d, y is %d\n", j, i);
					for(int x = 0; x < (int)list.size(); x++)
					{
						if (list[x].topX == j || ((j > list[x].topX) && (j < (list[x].topX + list[x].width + 2))))
						{	
							if (list[x].topY == i || (i > list[x].topY) && (i < (list[x].topY + list[x].height + 2)))
							{
								j += list[x].width;
								flag = 1;
								//printf("here\n");
								break;
							}
						}
					}
					if(flag == 1)
					{
						flag = 0;
						continue;
					}
					else
					{
						firstx = j;
						firsty = i;
						printf("next black pixle is at x %d y %d\n", firstx, firsty);
						tempPtr = src.data(firstx, firsty);
						for (int x = 0; x < width; x++)
						{
							tempPtr = src.data(firstx+x, firsty);
							//printf("%d , %d, %d, %d \n", tempPtr[0],tempPtr[1], tempPtr[2], tempPtr[3]);
							if(tempPtr[0] != 0 || tempPtr[1] !=0 || tempPtr[2] != 0 || tempPtr[3] != 0)
								break;
							tempWidth = x;
						}
						tempPtr = src.data(firstx, firsty);
						for (int y = 0; y < height; y++)
						{
							tempPtr = src.data(firstx, firsty+y);
							if(tempPtr[0] != 0 || tempPtr[1] !=0 || tempPtr[2] != 0 || tempPtr[3] != 0)
								break;
							tempHeight = y;
						}
						block.height = tempHeight;
						block.width = tempWidth;
						block.topX = firstx;
						block.topY = firsty;
						list.push_back(block);
						printf("there are %d platforms\n", list.size());
						printf("temp height = %d, temp width = %d\n", tempHeight, tempWidth);
						j += tempWidth;
					}
				}
				//count++;
			}	
		}
	}
	for (int i = 0; i < (int)list.size(); i++)
	{
		printf("platform[%d] x = %d, y = %d, height = %d, width = %d\n", i+1, list[i].topX, list[i].topY, list[i].height, list[i].width);
	}
	// this is the part where i make the damn XML thing
	
    //ss.str(""); keep in mind this is how to you clear a stingstream!!

	std::stringstream tag;
	std::stringstream xValue;
	std::stringstream yValue;
	std::stringstream widthValue;
	std::stringstream heightValue;
	std::stringstream id;
	std::stringstream main;
	xmlwriter worldList("level4.xml");

	for(int i = 0; i < (int)list.size(); i++)
	{
		tag << "platform";
		xValue << list[i].topX;
		yValue << list[i].topY;
		widthValue << list[i].width;
		heightValue << list[i].height;
		id << i;

		worldList.AddAtributes("width ", widthValue.str());
		worldList.AddAtributes("height ", heightValue.str());
		worldList.AddAtributes("Y ", yValue.str());
		worldList.AddAtributes("X ", xValue.str());
		worldList.AddAtributes("id ", id.str());
		worldList.Createtag(tag.str());
		worldList.CloseLasttag();

		id.str("");
		tag.str("");
		xValue.str("");
		yValue.str("");
		widthValue.str("");
		heightValue.str("");
	}

	for(int i = 0; i < (int)character.size(); i++)
	{
		tag << "moveable";
		xValue << character[i].topX;
		yValue << character[i].topY;
		widthValue << character[i].width;
		heightValue << character[i].height;
		main << character[i].main; 
		id << i;

		worldList.AddAtributes("main ", main.str());
		worldList.AddAtributes("width ", widthValue.str());
		worldList.AddAtributes("height ", heightValue.str());
		worldList.AddAtributes("Y ", yValue.str());
		worldList.AddAtributes("X ", xValue.str());
		worldList.AddAtributes("id ", id.str());
		worldList.Createtag(tag.str());
		worldList.CloseLasttag();

		id.str("");
		tag.str("");
		main.str("");
		xValue.str("");
		yValue.str("");
		widthValue.str("");
		heightValue.str("");
	}
	worldList.CloseAlltags();

}
	

