#include "ISquishable.h"

ISquishable::ISquishable(float x, float y) : CGameplayElement(PhysicsEngineObject, SceneObject, Area), xScale(x), yScale(y) {
}

SVector2 ISquishable::Squish(SVector2 velocity) {
   SVector2 scale;

   //If the player is grounded, slowly revert back to normal.
   if (velocity.Y < .01f && velocity.Y > -.01f) {
      if (xScale < 2.0f) {
         xScale += .002f;
         yScale -= .002f;
      }

      else if (xScale > 2.0f) {
         xScale -= .002f;
         yScale += .002f;
      }
   }

   else if (velocity.Y > .01f) { //When jumping/launched up
      if (yScale < 2.4f && xScale > 1.6f) {
         yScale += .0008f*velocity.Y;
         xScale -= .0008f*velocity.Y;
      }
   }

   else if (velocity.Y < -0.01f) { //When falling down
      if (xScale < 2.4f && yScale > 1.6f) {
         yScale -= -1.0f*.0008f*velocity.Y;
         xScale += -1.0f*.0008f*velocity.Y;
      }
   }

   scale.X = xScale;
   scale.Y = yScale;

   return scale;
}
