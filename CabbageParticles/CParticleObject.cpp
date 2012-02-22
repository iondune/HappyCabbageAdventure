#include "CParticleObject.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"
CRenderable * CParticleObject::getParticlesRenderable() {return particlesRenderable;}
void CParticleObject::setPositions(SVector3 *vectorArr) {
   updated = false;
   positionsArr = vectorArr;
}

/*
void CMeshSceneObject::update()
{
   ISceneObject::update();
}
*/

void CParticleObject::update() {
   ISceneObject::update();

   if(updated)
      return;

   updated = true;
   PositionBuffer.clear();
   for(int i = 0; i < numParticles; i++) {
      for(int j = 0; j < 3; j++) {
         PositionBuffer.push_back(positionsArr[i][j]);
      }
   }
}

void CParticleObject::setup(SVector3 * vectorArr, SVector3 * colorArr, float * sizeArr, int num) {
   updated = true;
   numParticles = num;
   particlesRenderable = new CRenderable(this);

   positionsArr = vectorArr;
   for(int i = 0; i < numParticles; i++) {
      for(int j = 0; j < 3; j++) {
         PositionBuffer.push_back(vectorArr[i][j]);
         ColorBuffer.push_back(colorArr[i][j]);
         SizeBuffer.push_back(sizeArr[i]);
      }
   }

   // Add attributes and uniforms
   particlesRenderable->addAttribute("aPosition", new SAttribute<float>(& PositionBuffer, 3));
   particlesRenderable->addAttribute("aColor", new SAttribute<float>(& ColorBuffer, 3));
   particlesRenderable->addAttribute("aPointSize", new SAttribute<float>(& SizeBuffer, 3));
   particlesRenderable->addUniform("uTexColor", new SUniform<int>(0));

   this->setTexture(CImageLoader::loadTexture("Textures/particle.bmp"));
   particlesRenderable->setShader(CShaderLoader::loadShader("Particle"));

   // Add mesh index buffer
   particlesRenderable->setIndexBufferObject(& IndexBuffer);

   // Reset the shader to load attributes again
   particlesRenderable->reloadVariablesOnNextDraw();
}
