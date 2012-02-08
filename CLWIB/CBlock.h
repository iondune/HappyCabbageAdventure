#ifndef __CBLOCK_H____
#define __CBLOCK_H____

//#include "CPlaceable.h"
#include <stdio.h>

class CBlock// : public CPlaceable
{
   float x,y;
   int w,h;
   public:
   CBlock(float nx, float ny, int width, int height);
   void printXML();
   void moveTo(float,float);
};

#endif
