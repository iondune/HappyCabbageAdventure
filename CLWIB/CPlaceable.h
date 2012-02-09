#ifndef __CPLACEABLE_H____
#define __CPLACEABLE_H____
class CPlaceable {
   public:
   float x,y;
   int w,h;

      virtual void printXML() =0;
      virtual void moveTo(float,float) =0;
};
#endif
