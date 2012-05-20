#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"
#include "ISceneObject.h"

#include "CMaterial.h"
#include "SAttribute.h"
#include "SUniform.h"


/*!
 * A CRenderable is some collection of buffer objects which can be drawn by a single OpenGL draw call.
 */
class CRenderable
{

protected:

    // Implicit shader variables
    glm::mat4 ModelMatrix, NormalMatrix;
	SUniform<glm::mat4> BindModelMatrix, BindNormalMatrix;

	// Local shader variables
    std::map<std::string, boost::shared_ptr<IAttribute const> > Attributes;
    std::map<std::string, boost::shared_ptr<IUniform const> > Uniforms;

	// Loaded shader variables
	std::map<GLint, boost::shared_ptr<IAttribute const> > LoadedAttributes;
    std::map<GLint, boost::shared_ptr<IUniform const> > LoadedUniforms;

	// Required data for drawing
	CShader * Shader[ERP_COUNT];
    CBufferObject<GLushort> * IndexBufferObject;

	// Material attribute used by phong light, etc.
    CMaterial Material;
	STransformation3 Transformation;

    static CShader * NormalColorShader;
    CRenderable * NormalObject;

    GLenum DrawType;

    void loadShaderVariables(CShader const * const shader, CScene const * const scene);
    CScene const * LastLoadedScene;
    CShader const * LastLoadedShader;

	ISceneObject * Parent;

	std::vector<GLenum> RenderCapabilities;
	std::vector<GLenum> RemovedRenderCapabilities;

	int DebugCounterThing;

public:

	int Size;
    CRenderable(ISceneObject * parent);

    CMaterial & getMaterial();
    CMaterial const & getMaterial() const;

    CBufferObject<GLushort> * getIndexBufferObject();
    void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

	CShader * getShader(ERenderPass const Pass);
	void setShader(ERenderPass const Pass, CShader * shader);

    GLenum const getDrawType() const;
    void setDrawType(GLenum const drawType);

    virtual void draw(CScene const * const scene, ERenderPass const Pass);
	virtual void drawNormals(CScene const * const scene);

	template <typename T>
	void addUniform(std::string const & label, T const & uniform)
	{
		Uniforms[label] = boost::shared_ptr<SUniform<T> >(new SUniform<T>(uniform));
	}

	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute);
	void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform);

    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

	boost::shared_ptr<IAttribute const> const getAttribute(std::string const & label);
	boost::shared_ptr<IUniform const> const getUniform(std::string const & label);

	void load(CScene const * const Scene, ERenderPass const Pass);
	CShader * updateShaderVariables(CScene const * const Scene, ERenderPass const Pass);
	
    CRenderable * & getDebuggingNormalObject();

	void reloadVariablesOnNextDraw();

	void addRenderCapability(GLenum const capability);
	void removeRenderCapability(GLenum const capability);

};

#endif
