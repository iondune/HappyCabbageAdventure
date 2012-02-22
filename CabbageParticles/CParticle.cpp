#include "CParticle.h"

void CParticle::setCenterPos(SVector3 *cPos) {
   centerPos = cPos;
}

void CParticle::setLookRight(int *ptr) {
   lookRight = ptr;
}

void CParticle::setAppearRate(float r) {
   Duration = 0;
   Counter = 0;
   AppearRate = r;
}

