#ifndef __CPLACEABLE_H____
#define __CPLACEABLE_H____
#include <sstream>
#include <string>
class CPlaceable {
   public:
   float x,y;
   int w,h;

      virtual void printXML() =0;
      virtual void moveTo(float,float) =0;
      virtual std::string tag();
      std::string getX();
      std::string getY();
      std::string getWidth();
      std::string getHeight();
};
#endif
