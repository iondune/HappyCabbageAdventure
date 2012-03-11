#ifndef _CABBAGECOLLIDER_CSCENEEFFECTMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CSCENEEFFECTMANAGER_H_INCLUDED_

#include <vector>
#include <map>

#include "CFrameBufferObject.h"
#include "ISceneObject.h"

class CSceneEffectManager
{

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

	bool DoSSAO, OnlySSAO, DoBloom, DoBlur, OnlyNormals;

};

#endif
