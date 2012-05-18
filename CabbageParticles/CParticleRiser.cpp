#include "CParticleRiser.h"

CParticleRiser::CParticleRiser(float maxHeight, float minSpeed, float maxSpeed) : globalMaxHeight(maxHeight), globalMinSpeed(minSpeed), globalMaxSpeed(maxSpeed) {

}

void CParticleRiser::setupRenderable() {
   localMaxHeight = globalMaxHeight - frand() * globalMaxHeight / 5.0f;
   yValue = 0.0f;
   ySpeed = frand()*(globalMaxSpeed - globalMinSpeed) + globalMinSpeed;
}

void CParticleRiser::updateMatrices(float time) {
   yValue += ySpeed * time;
   if(yValue > localMaxHeight) {
      yValue = 0.0f;
   }
}
