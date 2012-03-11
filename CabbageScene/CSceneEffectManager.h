#ifndef _CABBAGECOLLIDER_CSCENEEFFECTMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEEFFECTMANAGER_H_INCLUDED_

#include <vector>
#include <map>

#include "CFrameBufferObject.h"
#include "ISceneObject.h"

enum ESceneEffect
{
	ESE_SSAO = 1,
	ESE_SSAO_BLUR = 2,
	ESE_BLOOM = 4
};

class CSceneEffectManager
{

	int EnabledEffects;

public:

	CSceneEffectManager();

	class SRenderPass
	{

	public:

		CFrameBufferObject * Target;
		ERenderPass Pass;
	
	};

	class SPostProcessPass
	{

	public:
		
		SPostProcessPass();

		CShader * Shader;
		CFrameBufferObject * Target;

		std::map<std::string, float> Floats;
		std::map<std::string, int> Ints;
		std::map<std::string, CTexture *> Textures;

		void doPass();

	};

	std::vector<SRenderPass> RenderPasses;

	virtual void apply();

	void setEffectEnabled(ESceneEffect const Effect, bool const Enabled);
	bool const isEffectEnabled(ESceneEffect const Effect);

};

#endif
