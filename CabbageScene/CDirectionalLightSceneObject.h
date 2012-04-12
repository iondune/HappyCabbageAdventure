#ifndef _CABBAGE_SCENE_CDIRECTIONALLIGHTSCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CDIRECTIONALLIGHTSCENEOBJECT_H_INCLUDED_

#include "ISceneObject.h"
#include "CMeshLoader.h"

/*!
 */
class CDirectionalLightSceneObject : public ISceneObject
{

protected:

	CShader * Shader;
	SColor Color;
	SVector3 Direction;

public:

	CDirectionalLightSceneObject(SVector3 const direction, SColor const & color = SColor(1,1,1));

    void draw(CScene const * const scene, ERenderPass const Pass);

	virtual void load(CScene const * const Scene);

	void setDirection(SVector3 const & direction);
	void setColor(SVector3 const & color);

};

#endif
