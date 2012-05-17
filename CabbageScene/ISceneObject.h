#ifndef _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_

#include <map>
#include <set>
#include <list>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"

class EDebugData
{

public:

	enum Domain
	{
		None = 0,
		Normals = 1,
		NormalColors = 2
	};

};

enum ERenderPass
{
	ERP_DEFAULT,
	ERP_MODEL_NORMALS,
	ERP_DEFERRED_OBJECTS,
	ERP_DEFERRED_LIGHTS,
	ERP_COUNT
};

class CScene;

class ISceneObject
{

	static int ObjectsCulled, TotalObjects, CullChecks;

protected:

	// Model Transformation
	STransformation3 Transformation;
	glm::mat4 AbsoluteTransformation;
	glm::mat4 RotationMatrix;
	SBoundingBox3 BoundingBox;

	SVector3f Rotation, Translation, Scale;

	int DebugDataFlags;

	bool Visible;

	std::list<ISceneObject *> Children;
	ISceneObject * Parent;

	bool UseCulling;
	bool Immobile; // Its transforms will never change after the algorithm has ran

public:

	bool const isImmobile() const;
	void setImmobile(bool isImmobile);
	ISceneObject();

	void updateAbsoluteTransformation();
	glm::mat4 const & getAbsoluteTransformation() const;

	void setTreeImmobile(bool value);
	void setTranslation(SVector3f const & translation);
	void setRotation(SVector3f const & rotation);
	void setRotation(glm::mat4 const & matrix);
	void setScale(SVector3f const & scale);

	virtual void update();
	virtual void draw(CScene const * const scene, ERenderPass const Pass);
	int getNumLeaves();

	SBoundingBox3 const & getBoundingBox() const;
	SBoundingBox3 & getBoundingBox();
	void setBoundingBox(SBoundingBox3 const & boundingBox);

	bool const isDebugDataEnabled(EDebugData::Domain const type) const;
	void enableDebugData(EDebugData::Domain const type);
	void disableDebugData(EDebugData::Domain const type);

	bool const intersectsWithLine(SLine3 const & line) const;

	bool const isVisible() const;
	void setVisible(bool const isVisible);

	STransformation3 const & getTransformation() const;

	ISceneObject const * const getParent() const;
	std::list<ISceneObject *> const & getChildren() const;

	void removeChild(ISceneObject * child);
	void addChild(ISceneObject * child);
	void setParent(ISceneObject * parent);
	void removeChildren();
	void sortChildrenByZTranslation();

	SVector3f const & getRotation() const;
	SVector3f const & getTranslation() const;
	SVector3f const & getScale() const;

	bool const isCulled(CScene const * const Scene) const;
	bool const isCullingEnabled() const;
	void setCullingEnabled(bool const culling);

	virtual void load(CScene const * const Scene, ERenderPass const Pass);
	SVector3f getWorldBoundingBoxMinPoint();
	SBoundingBox3 getWorldBoundingBox();

	static void resetObjectCounts();

	static int const getObjectsCulled();
	static int const getTotalObjects();
	static int const getCullChecks();

};

#endif
