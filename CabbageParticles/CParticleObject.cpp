#include "CParticleObject.h"

#include "CShaderContext.h"
#include "CShaderLoader.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"
CRenderable * CParticleObject::getParticlesRenderable() {return particlesRenderable;}
void CParticleObject::setPositions(std::vector<SVector3f*> vectorArr) {
   updated = false;
   positionsArr = vectorArr;
}

/*
void CMeshSceneObject::update()
{
   ISceneObject::update();
}
*/

bool CParticleObject::getOnlyRenderLights() {
   return OnlyRenderLights;
}

void CParticleObject::setOnlyRenderLights(bool b) {
   OnlyRenderLights = b;
}

void CParticleObject::update() {
   ISceneObject::update();

   if(updated || !this->isVisible())
      return;

   updated = true;
   PositionBuffer.clear();
   for(int i = 0; i < numParticles; i++) {
      for(int j = 0; j < 3; j++) {
         PositionBuffer.push_back((*(positionsArr[i]))[j]);
      }
      LightsArr[i]->setTranslation(*(positionsArr[i]));
      //LightsDArr[i]->setTranslation(*(positionsArr[i]));
   }
}

void CParticleObject::draw(CScene const * const scene, ERenderPass const Pass)
{
   // The next two lines make it so that if the particle object is culled, none of the children particles will even be checked for culling
   if(ISceneObject::isCulled(scene) || !this->isVisible())
      return;
   ISceneObject::draw(scene, Pass);

   switch (Pass)
   {
   case ERP_DEFAULT:
   case ERP_DEFERRED_OBJECTS:
      if(!OnlyRenderLights) {
         glEnable(GL_POINT_SPRITE);
         glDepthMask(GL_FALSE);
         GLint oldAlpha, oldRGB;
         glGetIntegerv(GL_BLEND_DST_ALPHA, &oldAlpha);
         glGetIntegerv(GL_BLEND_DST_RGB, &oldRGB);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         particlesRenderable->draw(scene, Pass);
         glBlendFunc(oldAlpha, oldRGB);
         //glDepthMask(GL_TRUE);
         glDisable(GL_POINT_SPRITE);
         glDepthFunc(GL_LESS);
      }
      break;
   case ERP_MODEL_NORMALS:
   case ERP_DEFERRED_LIGHTS:
      break;
   }
   //glEnable(GL_DEPTH_TEST);
}

void CParticleObject::setAlwaysRender() {
   setCullingEnabled(false);
}

CParticleObject::CParticleObject() : OnlyRenderLights(false) {
   sizeFactor = 40.0f;
}

void CParticleObject::setSizeFactor(float f) {
   sizeFactor = f;
}

void CParticleObject::setup(std::vector<SVector3f*> vectorArr, std::vector<SVector3f*> colorArr, std::vector<float> sizeArr, int num, const char* texturePath) {
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
      CPointLightSceneObject *LightObj = new CPointLightSceneObject(sizeArr[i]/sizeFactor, *(colorArr[i]));
      CDirectionalLightSceneObject *LightDObj = new CDirectionalLightSceneObject(SVector3f(0, 0, 1), *(colorArr[i]));
      addChild(LightObj);
      //addChild(LightDObj);
      LightsArr.push_back(LightObj);
      LightObj->setCullingEnabled(false);
      //LightsDArr.push_back(LightDObj);

      IndexBuffer.push_back((unsigned short)i);
   }
   IndexBuffer.setIsIndexBuffer(true);

   // Add attributes and uniforms
   particlesRenderable->addAttribute("aPosition", boost::shared_ptr<IAttribute>(new SAttribute<float>(& PositionBuffer, 3)));
   particlesRenderable->addAttribute("aColor", boost::shared_ptr<IAttribute>(new SAttribute<float>(& ColorBuffer, 3)));
   particlesRenderable->addAttribute("aPointSize", boost::shared_ptr<IAttribute>(new SAttribute<float>(& SizeBuffer, 3)));
	static int const TexLevel = 0;
   particlesRenderable->addUniform("uTexColor", TexLevel);

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
