#ifndef __MATERIALS_HEADER_FILE_
#define __MATERIALS_HEADER_FILE_

#include <GL/gl.h>

#define LIGHT_GREEN_MATTE 1
#define DARK_GREEN_MATTE 2
#define BROWN_MATTE 3

//Increment this when you add a new material!
#define NUM_MATERIALS 3

void setMaterial(int num);

#endif
