#include "CPCube.h"

void CPCube::setupRenderable() {
   sineValue = 0;
   renderable = new CParticleRenderable();
   CMesh * cube = CMeshLoader::loadAsciiMesh("Cube");
   cube->calculateNormalsPerFace();
   renderable->setMesh(cube);
   renderable->setScale(0.07);
   renderable->centerPos = centerPos;
   renderable->getMaterial().Texture = CImageLoader::loadTexture("Textures/sky.bmp");
   renderable->getMaterial().Shader = CShaderLoader::loadShader("DiffuseTextureBright");

   yFactor = (float)rand()/(float)RAND_MAX * 1.5 - 0.4;
   Amplitude = 0.4*(yFactor + 1);
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
   RotationSpeed = SVector3(
         (float)rand()/(float)RAND_MAX * 4 - 2, 
         (float)rand()/(float)RAND_MAX * 4 - 2,
         (float)rand()/(float)RAND_MAX * 4 - 2);
   RotationSpeed *= -yFactor + 2.0;
   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   Counter = StartFactor;
}

void CPCube::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration >= TotalDuration - (float)AppearRate*0.85) {
      CApplication::get().getSceneManager().removeSceneObject(getRenderable());
      Duration = -1;
   }
   else if(Duration == -1) {}
   else {
      sineValue += 4*timeElapsed;
      Duration += timeElapsed;
      float newX = (*lookRight?1:-1)*(1+yFactor);
      getRenderable()->setTranslation(
            SVector3(
               newX,
               Amplitude*cos(Period*sineValue), 
               Amplitude*sin(Period*sineValue)));
      getRenderable()->setRotation(RotationSpeed*sineValue);
   }
}
