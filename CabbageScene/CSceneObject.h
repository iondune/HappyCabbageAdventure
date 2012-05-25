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

	std::vector<GLenum> RenderCapabilities;
	std::vector<GLenum> RemovedRenderCapabilities;

    GLenum DrawType;
	
	CShader * Shader[ERenderPass::Count];

    void loadShaderVariables(CShader const * const shader, CScene const * const scene);
    CScene const * LastLoadedScene;
    CShader const * LastLoadedShader;

	// Local shader variables
    std::map<std::string, boost::shared_ptr<IAttribute const> > Attributes;
    std::map<std::string, boost::shared_ptr<IUniform const> > Uniforms;

	// Loaded shader variables
	std::map<std::pair<GLint, std::string>, boost::shared_ptr<IAttribute const> > LoadedAttributes;
    std::map<std::pair<GLint, std::string>, boost::shared_ptr<IUniform const> > LoadedUniforms;

public:

	CSceneObject();

	void addRenderable(CRenderable * Renderable);
	void removeRenderable(CRenderable * Renderable);

    void draw(CScene const * const scene, ERenderPass const Pass);

	void setShader(ERenderPass const Pass, CShader * Shader);
	void setShader(ERenderPass const Pass, std::string const & Shader);

	void setMaterial(CMaterial const & Material);

	void setTexture(unsigned int const Layer, CTexture * Texture);
	void setTexture(unsigned int const Layer, CTexture * Texture, unsigned int const Renderable);
	void setTexture(unsigned int const Layer, std::string const & Texture);
	void setTexture(unsigned int const Layer, std::string const & Texture, unsigned int const Renderable);

	template <typename T>
	void addUniform(std::string const & label, T const & uniform)
	{
		for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
			(* it)->addUniform(label, uniform);
	}

	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute);
    void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform);

    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

	CShader const * const getShader() const;
	CShader * getShader();

	CShader const * const getShader(ERenderPass const Pass) const;
	CShader * getShader(ERenderPass const Pass);

	virtual void load(CScene const * const Scene, ERenderPass const Pass);

	void addRenderCapability(GLenum const capability);
	void removeRenderCapability(GLenum const capability);

};

#endif
