#ifndef _CABBAGE_SCENE_CSCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CSCENEOBJECT_H_INCLUDED_

#include "ISceneObject.h"
#include "CRenderable.h"

/*!
 * Standard implementation of ISceneObject, renders a set of CRenderables
 */
class CSceneObject : public ISceneObject
{

protected:

	std::vector<CRenderable *> Renderables;

public:

	CSceneObject();

	void addRenderable(CRenderable * Renderable);
	void removeRenderable(CRenderable * Renderable);

	void setShader(CShader * shader);
	void setMaterial(CMaterial const & material);

};

#endif
