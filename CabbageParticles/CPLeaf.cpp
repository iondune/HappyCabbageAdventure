#include "CPLeaf.h"

void CPLeaf::setupRenderable() {
   sineValue = 0;
   /*
   renderable = new CMeshSceneObject();
   CMesh * cube = CMeshLoader::loadAsciiMesh("Disc");
   cube->calculateNormalsPerFace();
   renderable->setMesh(cube);
   renderable->setScale(SVector3(0.13, 0.26, 0.13));
   renderable->setTranslation(* centerPos);
   CMaterial mat;
   mat.Texture = CImageLoader::loadTexture("Textures/grass.bmp");
   renderable->setMaterial(mat);
   renderable->setShader(CShaderLoader::loadShader("DiffuseTextureBright"));
   */

   yFactor = (float)rand()/(float)RAND_MAX * 0.6 - 0.3;
   Amplitude = (float)rand()/(float)RAND_MAX * 0.5 + 0.3;
   Period = (float)rand()/(float)RAND_MAX * 2 + 1;
   RotationSpeed = SVector3((float)rand()/(float)RAND_MAX * 4 - 2, 
         (float)rand()/(float)RAND_MAX * 4 - 2,
         (float)rand()/(float)RAND_MAX * 4 - 2);
   StartFactor = (float)rand()/(float)RAND_MAX * AppearRate; 
   Counter = StartFactor;
}

void CPLeaf::updateMatrices(float timeElapsed) {
   if(Counter > 0) {
      Counter -= timeElapsed;
   }
   else if(Duration >= TotalDuration - (float)AppearRate*0.85) {
      //CApplication::get().getSceneManager().removeSceneObject(getRenderable());
      Duration = -1;
   }
   else if(Duration == -1) {}
   else {
      sineValue += 4*timeElapsed;
      Duration += timeElapsed;
      translate = SVector3(Amplitude*cos(Period*sineValue),
                           yFactor,
                           Amplitude*sin(Period*sineValue));
      //getRenderable()->setRotation(RotationSpeed*sineValue*5);
   }
}
