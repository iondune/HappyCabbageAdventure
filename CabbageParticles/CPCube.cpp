#include "CPCube.h"

void CPCube::setupRenderable() {
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
   StartFactor = (float)rand()/(float)RAND_MAX * 180 - 90; 
}

void CPCube::updateMatrices(float sineValue) {
   float mSineValue = sineValue + StartFactor;
   getRenderable()->setTranslation(
         SVector3(
            0.5+yFactor, 
            Amplitude*cos(Period*mSineValue), 
            Amplitude*sin(Period*mSineValue)));

   getRenderable()->setRotation(RotationSpeed*mSineValue);
}
