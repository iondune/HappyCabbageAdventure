#include "ISquishable.h"

ISquishable::ISquishable(float x, float y) : xScale(x), yScale(y), xScaleOrig(x), yScaleOrig(y) {
}

SVector2f ISquishable::Squish(SVector2f velocity) {
   SVector2f scale;

   //If the player is grounded, slowly revert back to normal.
   if (velocity.Y < .01f && velocity.Y > -.01f) {
      if (xScale < xScaleOrig + .01f) {
         xScale += .002f;
         yScale -= .002f;
      }

      else if (xScale > xScaleOrig - .01f) {
         xScale -= .002f;
         yScale += .002f;
      }
   }

   else if (velocity.Y > .01f) { //When jumping/launched up
      if (yScale < yScaleOrig + .2f*yScaleOrig && xScale > xScaleOrig - .2f*xScaleOrig) {
         yScale += .0008f*velocity.Y;
         xScale -= .0008f*velocity.Y;
      }
   }

   else if (velocity.Y < -0.01f) { //When falling down
      if (xScale < xScaleOrig + .2f*xScaleOrig && yScale > yScaleOrig - .2f*yScaleOrig) {
         yScale -= -1.0f*.0008f*velocity.Y;
         xScale += -1.0f*.0008f*velocity.Y;
      }
   }

   scale.X = xScale;
   scale.Y = yScale;

   return scale;
}
