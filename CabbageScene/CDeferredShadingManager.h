#ifndef _CABBAGECOLLIDER_CDEFERREDSHADINGMANAGER_H_INCLUDED_
#define _CABBAGECOLLIDER_CDEFERREDSHADINGMANAGER_H_INCLUDED_

#include "CSceneEffectManager.h"

class CDeferredShadingManager : public CSceneEffectManager
{

public:

	CFrameBufferObject * DeferredOutputTarget;
	CTexture * DeferredColorOutput;
	CTexture * DeferredPositionOutput;
	CTexture * DeferredNormalOutput;

	CFrameBufferObject * LightPassOutputTarget;
	CTexture * LightPassColorOutput;

	CShader * FinalBlendShader;

	CTexture * WaterOffsetTexture;

	CDeferredShadingManager(CSceneManager * sceneManager);

	virtual void apply();

	virtual void setEffectEnabled(ESceneEffect const Effect, bool const Enabled);
	virtual bool const isEffectEnabled(ESceneEffect const Effect);

};

#endif
