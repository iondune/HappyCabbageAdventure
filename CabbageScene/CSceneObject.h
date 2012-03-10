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

	void addSceneObject(CRenderable * Renderable);
	void removeSceneObject(CRenderable * Renderable);

    void draw(CScene const * const scene, ERenderPass const Pass);

	void setShader(CShader * shader);
	void setShader(std::string const & shader);
	void setMaterial(CMaterial const & material);
	void setTexture(CTexture * texture);
	void setTexture(CTexture * texture, int const renderable);
	void setTexture(std::string const & texture);
	void setTexture(std::string const & texture, int const renderable);

	CShader const * const getShader() const;
	CShader * getShader();

	virtual void load(CScene const * const Scene);

};

#endif
