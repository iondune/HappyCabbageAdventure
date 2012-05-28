#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

// STL
#include <map>
#include <set>
#include <vector>

// Core
#include <boost/shared_ptr.hpp>
#include <SVector3.h>
#include <ionEnum.h>
#include <SColor.h>
#include <STransformation3.h>

// Scene
/*#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"
#include "ISceneObject.h"

#include "CMaterial.h"
#include "SAttribute.h"
#include "SUniform.h"*/

// TODO: Remove
#ifdef __unix__
#include<GL/gl.h>
#include<GL/glu.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#endif

#include "ISceneObject.Enumerations.h"


struct EDrawElementTypeValues
{
	enum Domain
	{
		Points,
		Lines,
		Triangles,
		Quads
	};
};

typedef Enum<EDrawElementTypeValues> EDrawElementType;


// Forward Declarations
template <typename T>
class CBufferObject;
class CTexture;
class IAttribute;
class IUniform;
class IScene;
class CSceneObject;

//! 
//! A CRenderable is some collection of buffer objects which can be drawn by a single OpenGL draw call.
//! 
class CRenderable
{

public:

	//! Material attributes used for rendering (typically as uniform shader variables)
	struct SMaterial
	{

	public:

		std::vector<CTexture *> Textures;

		SColor AmbientColor, DiffuseColor;
		float Shininess;

		SMaterial();
		void loadDefaults();

	};

	// Local shader variables
	std::map<std::string, boost::shared_ptr<IAttribute const> > Attributes;
	std::map<std::string, boost::shared_ptr<IUniform const> > Uniforms;

	// Loaded shader variables
	std::map<std::pair<GLuint, std::string>, boost::shared_ptr<IAttribute const> > LoadedAttributes;
	std::map<std::pair<GLuint, std::string>, boost::shared_ptr<IUniform const> > LoadedUniforms;

protected:

	//! Implicit shader variables
	glm::mat4 ModelMatrix, NormalMatrix;

	//! Index buffer for indexed drawing
	CBufferObject<GLushort> * IndexBufferObject; // TODO: Appropriate type names for this! (or abstraction)

	//! Number of elements drawn when no IndexBufferObject specified
	unsigned int ElementCount;

	//! Material attribute used by phong light, etc.
	SMaterial Material;

	//! Local transformation within object space
	STransformation3 Transformation;

	//! Used for debug data rendering
	CRenderable * NormalObject;

	//! Encapsulating Scene Object
	CSceneObject * ParentObject;

	//! Type of elements to draw
	EDrawElementType DrawElementType;

public:

	//! Constructor
	CRenderable(CSceneObject * parent);


	////////////////
	// Attributes //
	////////////////

	SMaterial const & getMaterial() const;
	void setMaterial(SMaterial const &);

	CBufferObject<GLushort> * getIndexBufferObject();
	void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

	EDrawElementType const getDrawType() const;
	void setDrawType(EDrawElementType const drawType);


	////////////////////
	// Update Methods //
	////////////////////

	void draw(IScene const * const scene, ERenderPass const Pass);
	void load(IScene const * const Scene, ERenderPass const Pass);


	//////////////////////
	// Shader Varaibles //
	//////////////////////

	void addAttribute(std::string const & label, boost::shared_ptr<IAttribute const> const attribute);
	void addUniform(std::string const & label, boost::shared_ptr<IUniform const> const uniform);

	void removeAttribute(std::string const & label);
	void removeUniform(std::string const & label);

	boost::shared_ptr<IAttribute const> const getAttribute(std::string const & label);
	boost::shared_ptr<IUniform const> const getUniform(std::string const & label);


	////////////////
	// Debug Info //
	////////////////

	CRenderable * & getDebuggingNormalObject();

};

#endif
