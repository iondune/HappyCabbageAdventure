#ifndef _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_

// STL
#include <map>
#include <set>
#include <list>

// Core
#include <boost/shared_ptr.hpp>
#include <ionEnum.h>
#include <SVector3.h>
#include <SBoundingBox3.h>

// Scene
#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"

struct EDebugDataValues
{
	enum Domain
	{
		All = 0,
		Normals = 1,
		NormalColors = 2
	};
};

class EDebugData : public Enum<EDebugDataValues>
{};


struct ERenderPassValues
{
	enum Domain
	{
		Default,
		ModelSpaceNormals,
		DeferredColors,
		DeferredLights,
		Count
	};
};

class ERenderPass : public Enum<ERenderPassValues>
{};


struct ERenderCategoryValues
{
	enum Domain
	{
		SkyBox,
		Default,
		Transparent,
		PostProcess,
		Count
	};
};

class ERenderCategory : public Enum<ERenderCategoryValues>
{};


class CScene;

class ISceneObject
{

	static int ObjectsCulled, TotalObjects, CullChecks;

protected:

	// Model Transformation
	STransformation3 Transformation;
	glm::mat4 AbsoluteTransformation;

	SBoundingBox3 BoundingBox;
	SBoundingBox3 AbsoluteBoundingBox;

	// Keep vector form of transformations for easy access
	SVector3f Rotation, Translation, Scale;

	// Keep track of changes that require updates
	bool TransformationDirty;
	bool BoundingBoxDirty;

	// Bitfield of different debug-purposed data should be shown
	int DebugDataFlags;

	// Whether or not to draw this object and all its children
	bool Visible;

	std::list<ISceneObject *> Children;
	ISceneObject * Parent;

	// Should this object be checked for trivial rejection
	bool UseCulling;

	ERenderCategory RenderCategory;

public:

	ISceneObject();

	glm::mat4 const & getAbsoluteTransformation() const;
	STransformation3 const & getTransformation() const;

	void setTranslation(SVector3f const & translation);
	void setRotation(SVector3f const & rotation);
	void setRotation(glm::mat4 const & matrix);
	void setScale(SVector3f const & scale);
	
	SVector3f const & getRotation() const;
	SVector3f const & getTranslation() const;
	SVector3f const & getScale() const;


	SBoundingBox3 const & getBoundingBox() const;
	void setBoundingBox(SBoundingBox3 const & boundingBox);

	bool const isDebugDataEnabled(EDebugData const type) const;
	void enableDebugData(EDebugData const type);
	void disableDebugData(EDebugData const type);
	
	bool const intersectsWithLine(SLine3 const & line) const;

	bool const isVisible() const;
	void setVisible(bool const isVisible);

	ERenderCategory const getRenderCategory() const;
	void setRenderCategory(ERenderCategory const RenderCategory);

	
	ISceneObject const * const getParent() const;
	std::list<ISceneObject *> const & getChildren() const;

	void removeChild(ISceneObject * child);
	void addChild(ISceneObject * child);
	void setParent(ISceneObject * parent);
	void removeChildren();

	
	bool const isCulled(CScene const * const Scene) const;
	bool const isCullingEnabled() const;
	void setCullingEnabled(bool const culling);

	void checkAbsoluteTransformation();
	void updateAbsoluteTransformation();

	virtual void update();
	virtual void load(CScene const * const Scene, ERenderPass const Pass);
	virtual void draw(CScene const * const scene, ERenderPass const Pass);


	static void resetObjectCounts();

	static int const getObjectsCulled();
	static int const getTotalObjects();
	static int const getCullChecks();

};

#endif
