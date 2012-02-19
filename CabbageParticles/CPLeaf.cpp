#include "CPLeaf.h"

void CPLeaf::setupRenderable() {
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
   StartFactor = (float)rand()/(float)RAND_MAX * 180 - 90; 
}

void CPLeaf::updateMatrices(float sineValue) {
   float mSineValue = sineValue + StartFactor;
   getRenderable()->setTranslation(
         SVector3(
            Amplitude*cos(Period*mSineValue), 
            yFactor, 
            Amplitude*sin(Period*mSineValue)));
   getRenderable()->setRotation(RotationSpeed*mSineValue);
}
