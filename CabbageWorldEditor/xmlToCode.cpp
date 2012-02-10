#include "levelReader.h"
#include "irrxml-1.2\src\irrXML.cpp"

using namespace irr;
using namespace io;

void xmlLevReader(std::vector<plat> & list, std::vector<obj> & character, std::string const & level)
{
	// must motifiy to pass in 2 vectors ASK IAN or ALDEN
	plat block;
	block.topX = 0;
	obj move;
    IrrXMLReader* xml = createIrrXMLReader(level.c_str());	
	while (xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_TEXT:
			break;
		case EXN_ELEMENT:
			if(!strcmp("platform", xml->getNodeName()))
			{
				// id, X, Y, height, width / from 0,1,2 so on
				block.topX = xml->getAttributeValueAsInt(1);
				block.topY = xml->getAttributeValueAsInt(2);
				block.height = xml->getAttributeValueAsInt(3);
				block.width = xml->getAttributeValueAsInt(4);
				list.push_back(block);
				printf("yes\n");
			}	
			if(!strcmp("moveable", xml->getNodeName()))
			{	
				move.topX = xml->getAttributeValueAsInt(1);
				move.topY = xml->getAttributeValueAsInt(1);
				move.height = xml->getAttributeValueAsInt(1);
				move.width = xml->getAttributeValueAsInt(1);
				move.main = xml->getAttributeValueAsInt(1) != 0;
				character.push_back(move);
				printf("no\n");
			}
				break;
		}
	}
	for (int i = 0; i < (int)list.size(); i++)
	{
		printf("platform[%d] x = %d, y = %d, height = %d, width = %d\n", i+1, list[i].topX, list[i].topY, list[i].height, list[i].width);
	}
}
