#ifndef ISQUISHABLE_H_
#define ISQUISHABLE_H_
#include "CGameplayElement.h"

class ISquishable {

   private:
      float xScale, yScale;
      float xScaleOrig, yScaleOrig;

   public:
      SVector2f Squish(SVector2f velocity);

      ISquishable(float x, float y);
};

#endif /* ISQUISHABLE_H_ */
