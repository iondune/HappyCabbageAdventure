#ifndef __CPLACEABLE_H____
#define __CPLACEABLE_H____
class CPlaceable {
   public:
      virtual void printXML() =0;
      virtual void moveTo(float,float) =0;
};
#endif
