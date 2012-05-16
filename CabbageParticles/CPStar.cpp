#include "CPStar.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

void CPStar::setupRenderable() {
   xValue = (float)rand()/(float)RAND_MAX * 200.0f - 20.0f;
   yValue = (float)rand()/(float)RAND_MAX * 20.0f + 4.0f;
   zValue = (float)rand()/(float)RAND_MAX * 0.5f - 0.25f - 4.0f;
}

#define MIN(x,y) ((x>y)?y:x)

void CPStar::updateMatrices(float timeElapsed) {
   translate = SVector3f(xValue, yValue, zValue);
}
