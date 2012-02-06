#pragma once

#include "Cimg-1.4.9\CImg.h"
#include "wmlwriter_src\xmlwriter.h"
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/**
 * platform
 * int topLeft is the x point of the block
 * int topRight is the y point of the block
 */
struct plat {
	int topX;
	int topY;
	int width;
	int height;
	bool moveable;
};

struct obj {
	int topX;
	int topY;
	int width;
	int height;
	bool main;
};

void levReader();
void xmlLevReader(std::vector<plat> & list, std::vector<obj> & character, std::string const & level);
