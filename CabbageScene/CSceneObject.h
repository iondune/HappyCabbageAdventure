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

	//! Contained buffer object children
	std::vector<CRenderable *> Renderables;

	//! Shader for each render pass
	CShader * Shader[ERenderPass::Count];

	//! Local shader variables
	std::map<std::string, boost::shared_ptr<IAttribute const> > Attributes;
	std::map<std::string, boost::shared_ptr<IUniform const> > Uniforms;

	//! Loaded shader variables
	std::map<std::pair<GLint, std::string>, boost::shared_ptr<IAttribute const> > LoadedAttributes;
	std::map<std::pair<GLint, std::string>, boost::shared_ptr<IUniform const> > LoadedUniforms;

public:

	//! Constructor
	CSceneObject();

	///////////////////////////
	// Renderable Management //
	///////////////////////////

	void addRenderable(CRenderable * Renderable);
	void removeRenderable(CRenderable * Renderable);
	void removeRenderable(unsigned int const index);

	unsigned int const getRenderableCount();
	CRenderable * const getRenderable(unsigned int const index);
	CRenderable const * const getRenderable(unsigned int const index) const;


	//////////////////////////////////
	// Rendering Context Management //
	//////////////////////////////////

	void setMaterial(CRenderable::SMaterial const & Material);
	void setMaterial(CRenderable::SMaterial const & Material, unsigned int const Renderable);
	void setMaterial(CRenderable::SMaterial const & Material, CRenderable * const Renderable);

	void setTexture(unsigned int const Layer, CTexture * Texture);
	void setTexture(unsigned int const Layer, CTexture * Texture, unsigned int const Renderable);
	void setTexture(unsigned int const Layer, CTexture * Texture, CRenderable * const Renderable);
	void setTexture(unsigned int const Layer, std::string const & Texture);
	void setTexture(unsigned int const Layer, std::string const & Texture, unsigned int const Renderable);
	void setTexture(unsigned int const Layer, std::string const & Texture, CRenderable * const Renderable);

	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute);
	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute, unsigned int const Renderable);
	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute, CRenderable * const Renderable);
	void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform);
	void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform, unsigned int const Renderable);
	void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform, CRenderable * const Renderable);

	void removeAttribute(std::string const & label);
	void removeAttribute(std::string const & label, unsigned int const Renderable);
	void removeAttribute(std::string const & label, CRenderable * const Renderable);
	void removeUniform(std::string const & label);
	void removeUniform(std::string const & label, unsigned int const Renderable);
	void removeUniform(std::string const & label, CRenderable * const Renderable);


	///////////////////////
	// Shader Management //
	///////////////////////

	//! Deprecated. Returns Default Render Pass shader
	CShader const * const getShader() const;
	//! Deprecated. Returns Default Render Pass shader
	CShader * getShader();

	CShader const * const getShader(ERenderPass const Pass) const;
	CShader * getShader(ERenderPass const Pass);

	void setShader(ERenderPass const Pass, CShader * Shader);
	void setShader(ERenderPass const Pass, std::string const & Shader);


	////////////////////
	// Update Methods //
	////////////////////

	virtual void draw(CScene const * const scene, ERenderPass const Pass);
	virtual void load(CScene const * const Scene, ERenderPass const Pass);

};

#endif
