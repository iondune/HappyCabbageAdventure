#include "CPHurt.h"

void CPHurt::setupRenderable() {
   tickValue = 0;
   position = SVector3((float)rand()/(float)RAND_MAX * 1 - 0.5f, (float)rand()/(float)RAND_MAX * 0.3f, (float)rand()/(float)RAND_MAX * 1 - 0.5f);
   velocity = ((*centerPos - position) - *centerPos) * SVector3(-0.3f, -1.4f, -0.7f); 
}

void CPHurt::setCenterPos(SVector3 *cPos) {
   centerPos = cPos;
}

void CPHurt::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration >= TotalDuration - (float)AppearRate*0.85) {
      //CApplication::get().getSceneManager().removeSceneObject(getRenderable());
      Duration = -1;
   }
   else if(Duration == -1) {}
   else {
      tickValue += timeElapsed*0.1f;
      Duration += timeElapsed;


      velocity -= SVector3(0.0f, 0.08f, 0.0f)*tickValue;
      //velocity.X /= 1.05f;
      //velocity.Z /= 1.05f;
      position += velocity*tickValue;
      translate = position; 
      //getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
