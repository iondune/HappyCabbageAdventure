#ifndef _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CPARTICLEOBJECT_H_INCLUDED_

#include "../CabbageCore/SVector3.h"
#include "../CabbageScene/CabbageScene.h"

class CParticleObject : public CSceneObject
{
protected:
   bool OnlyRenderLights, RenderLights;
   CRenderable * particlesRenderable;
   CBufferObject<float> PositionBuffer, ColorBuffer, SizeBuffer;
   CBufferObject<unsigned short> IndexBuffer;
   std::vector<SVector3f*> positionsArr;
   std::vector<CPointLightSceneObject*> LightsArr;
   std::vector<CDirectionalLightSceneObject*> LightsDArr;
   bool updated, AlwaysRender;
   int numParticles;
   float sizeFactor;

   CShader * DefaultShader;
   CShader * DeferredShader;

public:
   CRenderable * getParticlesRenderable();
   CParticleObject();
   void setup(std::vector<SVector3f*> vectorArr, std::vector<SVector3f*> colorArr, std::vector<float> sizeArr, int num, const char*);
   void setPositions(std::vector<SVector3f*> vectorArr);
   void setAlwaysRender();
   void setSizeFactor(float f);
   void update();
   void draw(CScene const * const scene, ERenderPass const Pass);
   void setOnlyRenderLights(bool);
   bool getOnlyRenderLights();
   void setRenderLights(bool);
   bool getRenderLights();
};

#endif
