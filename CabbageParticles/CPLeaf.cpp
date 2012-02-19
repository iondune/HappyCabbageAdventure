#include "CPLeaf.h"

void CPLeaf::setupRenderable() {
   sineValue = 0;
   renderable = new CParticleRenderable();
   CMesh * cube = CMeshLoader::loadAsciiMesh("Disc");
   cube->calculateNormalsPerFace();
   renderable->setMesh(cube);
   renderable->setScale(SVector3(0.13, 0.26, 0.13));
   renderable->centerPos = centerPos;
   renderable->getMaterial().Texture = CImageLoader::loadTexture("Textures/grass.bmp");
   renderable->getMaterial().Shader = CShaderLoader::loadShader("DiffuseTextureBright");

   yFactor = (float)rand()/(float)RAND_MAX * 0.6 - 0.3;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.5 + 0.3;
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
   RotationSpeed = SVector3((float)rand()/(float)RAND_MAX * 4 - 2, 
         (float)rand()/(float)RAND_MAX * 4 - 2,
         (float)rand()/(float)RAND_MAX * 4 - 2);
   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   AppearRate = StartFactor;
}

void CPLeaf::updateMatrices(float timeElapsed) {
   if(AppearRate > 0) {
      AppearRate -= timeElapsed;
   }
   else {
      sineValue += 4*timeElapsed;
      getRenderable()->setTranslation(
            SVector3(
               Amplitude*cos(Period*sineValue), 
               yFactor, 
               Amplitude*sin(Period*sineValue)));
      getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
