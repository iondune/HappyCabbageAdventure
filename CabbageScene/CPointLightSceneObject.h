#ifndef _CABBAGE_SCENE_CPOINTLIGHTSCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CPOINTLIGHTSCENEOBJECT_H_INCLUDED_

#include "ISceneObject.h"
#include "CMeshLoader.h"

/*!
 */
class CPointLightSceneObject : public ISceneObject
{

protected:

	CMesh * SphereMesh;
	CMesh::SMeshBuffer * MeshBuffer;

	CShader * Shader;
	SColor Color;

public:

	CPointLightSceneObject(float const radius = 3.f, SColor const & color = SColor(1,1,1));

    void draw(CScene const * const scene, ERenderPass const Pass);

	virtual void load(CScene const * const Scene);

	void setRadius(float const Radius);
	void setScale(SVector3f const & scale);
	void setColor(SVector3f const & color);

};

#endif
