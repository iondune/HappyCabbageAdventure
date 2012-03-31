#include "CParticleObject.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"
CRenderable * CParticleObject::getParticlesRenderable() {return particlesRenderable;}
void CParticleObject::setPositions(std::vector<SVector3*> vectorArr) {
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
         PositionBuffer.push_back((*(positionsArr[i]))[j]);
      }
   }
}

void CParticleObject::draw(CScene const * const scene, ERenderPass const Pass)
{
   //glDisable(GL_DEPTH_TEST);
   glEnable(GL_POINT_SPRITE);
   glDepthMask(GL_FALSE);
   //glDepthFunc(GL_ALWAYS);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   ISceneObject::draw(scene, Pass);

   switch (Pass)
   {
   case ERP_DEFAULT:
   case ERP_DEFERRED_OBJECTS:
      particlesRenderable->draw(scene, Pass);
      break;

   case ERP_MODEL_NORMALS:
      break;

   case ERP_DEFERRED_LIGHTS:
      break;
   }
   glBlendFunc(GL_ONE, GL_MAX);
   glDepthMask(GL_TRUE);
   glDisable(GL_POINT_SPRITE);
   glDepthFunc(GL_LESS);
   //glEnable(GL_DEPTH_TEST);
}

void CParticleObject::setup(std::vector<SVector3*> vectorArr, std::vector<SVector3*> colorArr, std::vector<float> sizeArr, int num, const char* texturePath) {
   updated = true;
   numParticles = num;
   particlesRenderable = new CRenderable(this);

   positionsArr = vectorArr;
   for(int i = 0; i < numParticles; i++) {
      for(int j = 0; j < 3; j++) {
         PositionBuffer.push_back((*(vectorArr[i]))[j]);
         ColorBuffer.push_back((*(colorArr[i]))[j]);
         SizeBuffer.push_back(sizeArr[i]);
      }
      IndexBuffer.push_back((unsigned short)i);
   }
   IndexBuffer.setIsIndexBuffer(true);

   // Add attributes and uniforms
   particlesRenderable->addAttribute("aPosition", new SAttribute<float>(& PositionBuffer, 3));
   particlesRenderable->addAttribute("aColor", new SAttribute<float>(& ColorBuffer, 3));
   particlesRenderable->addAttribute("aPointSize", new SAttribute<float>(& SizeBuffer, 3));
   particlesRenderable->addUniform("uTexColor", new SUniform<int>(0));

   glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

   particlesRenderable->getMaterial().Texture = CImageLoader::loadTexture(texturePath);

   particlesRenderable->setShader(ERP_DEFAULT, CShaderLoader::loadShader("Particle"));
   particlesRenderable->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Particle"));

   // Add mesh index buffer
   particlesRenderable->setIndexBufferObject(& IndexBuffer);
   particlesRenderable->setDrawType(GL_POINTS);
   particlesRenderable->Size = num;


   // Reset the shader to load attributes again
   particlesRenderable->reloadVariablesOnNextDraw();
}
