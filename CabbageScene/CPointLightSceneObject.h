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

public:

	CPointLightSceneObject();

    void draw(CScene const * const scene, ERenderPass const Pass);

	virtual void load(CScene const * const Scene);

};

#endif
