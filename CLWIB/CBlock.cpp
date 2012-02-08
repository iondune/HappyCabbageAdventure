#include "CBlock.h"

CBlock::CBlock(float nx, float ny, int width, int height)
{
   x = nx; y = ny;
   w = width; h = height;
}

void CBlock::printXML() {
   printf("X: %0.2f Y: %0.2f W: %d H: %d\n", x,y,w,h);
}
void CBlock::moveTo(float x,float y) {
}
