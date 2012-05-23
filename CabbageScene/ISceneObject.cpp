#include "ISceneObject.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

//#include "CShaderLoader.h"
//#include "CSceneManager.h"


int ISceneObject::ObjectsCulled = 0;
int ISceneObject::TotalObjects = 0;
int ISceneObject::CullChecks = 0;

void ISceneObject::resetObjectCounts()
{
	ObjectsCulled = 0;
	TotalObjects = 0;
	CullChecks = 0;
}

int const ISceneObject::getObjectsCulled()
{
	return ObjectsCulled;
}

int const ISceneObject::getTotalObjects()
{
	return TotalObjects;
}

int const ISceneObject::getCullChecks()
{
	return CullChecks;
}


ISceneObject::ISceneObject()
	: DebugDataFlags(0), Visible(true), Parent(0), UseCulling(true), TransformationDirty(false),
	RenderCategory(ERenderCategory::Standard)
{}

void ISceneObject::checkAbsoluteTransformation()
{
	if (TransformationDirty)
		updateAbsoluteTransformation();
	else
		for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
			(* it)->checkAbsoluteTransformation();
}

void ISceneObject::updateAbsoluteTransformation()
{
	AbsoluteTransformation = Transformation;

	if (Parent)
		AbsoluteTransformation = Parent->AbsoluteTransformation* AbsoluteTransformation;

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->updateAbsoluteTransformation();
}

glm::mat4 const & ISceneObject::getAbsoluteTransformation() const
{
	return AbsoluteTransformation;
}

void ISceneObject::setTranslation(SVector3f const & translation)
{
	Translation = translation;
	Transformation.setTranslation(translation);

	TransformationDirty = true;
}

void ISceneObject::setRotation(SVector3f const & rotation)
{
	Rotation = rotation;
	Transformation.setRotation(rotation);

	TransformationDirty = true;
}

void ISceneObject::setRotation(glm::mat4 const & matrix)
{
	Transformation.setRotation(matrix);

	TransformationDirty = true;
}

void ISceneObject::setScale(SVector3f const & scale)
{
	Scale = scale;
	Transformation.setScale(scale);

	TransformationDirty = true;
}

void ISceneObject::update()
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->update();
}

void ISceneObject::load(CScene const * const Scene, ERenderPass const Pass)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->load(Scene, Pass);
}

void ISceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return;
	
	++ TotalObjects;

	if (isCulled(scene)) // check absolute
		++ ObjectsCulled;
	else
		for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
			(* it)->draw(scene, Pass);
}

SBoundingBox3 const & ISceneObject::getBoundingBox() const
{
	return BoundingBox;
}

void ISceneObject::setBoundingBox(SBoundingBox3 const & boundingBox)
{
	BoundingBox = boundingBox;

	BoundingBoxDirty = true;
}

bool const ISceneObject::isDebugDataEnabled(EDebugData const type) const
{
	return (type & DebugDataFlags) != 0;
}

void ISceneObject::enableDebugData(EDebugData const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->enableDebugData(type);

	if (type == EDebugData::All)
		DebugDataFlags = -1;
	else
		DebugDataFlags |= type;
}

void ISceneObject::disableDebugData(EDebugData const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->disableDebugData(type);

	if (type == EDebugData::All)
		DebugDataFlags = 0;
	else
		DebugDataFlags ^= type;
}

bool const ISceneObject::intersectsWithLine(SLine3 const & line) const
{
	return BoundingBox.intersectsWithLine(line);
}

bool const ISceneObject::isVisible() const
{
	return Visible;
}

void ISceneObject::setVisible(bool const isVisible)
{
	// TO DO: Non visible objects should not be a part of hiearchies, so this should trigger a rebuild
	Visible = isVisible;
}

STransformation3 const & ISceneObject::getTransformation() const
{
	return Transformation;
}

ISceneObject const * const ISceneObject::getParent() const
{
	return Parent;
}

std::list<ISceneObject *> const & ISceneObject::getChildren() const
{
	return Children;
}

void ISceneObject::removeChild(ISceneObject * child)
{
	Children.erase(std::remove(Children.begin(), Children.end(), child), Children.end());
	child->Parent = 0;
}

void ISceneObject::addChild(ISceneObject * child)
{
	Children.push_back(child);
	child->Parent = this;
}

void ISceneObject::setParent(ISceneObject * parent)
{
	if (Parent)
		Parent->removeChild(this);

	if (parent)
		parent->addChild(this);
}

void ISceneObject::removeChildren()
{
	Children.clear();
}

SVector3f const & ISceneObject::getRotation() const
{
	return Rotation;
}

SVector3f const & ISceneObject::getTranslation() const
{
	return Translation;
}

SVector3f const & ISceneObject::getScale() const
{
	return Scale;
}

#include "CCamera.h"

bool const ISceneObject::isCulled(CScene const * const Scene) const
{
	static bool const Inside = false;
	static bool const Outside = true;
	++ CullChecks;

	if (! UseCulling || ! Scene->isCullingEnabled())
		return false;

	int i = 0;
	int in[6], out[6];

	for (int i = 0; i < 6; ++ i) 
	{
		in[i] = 0;
		out[i] = 0;
	}

	for (int i = 0; i < 8; ++ i) 
	{
		SVector3f const Center = getBoundingBox().getCorner(i);
		glm::vec4 Center4(Center.X, Center.Y, Center.Z, 1.f);

		glm::mat4 PVM = (Scene->getActiveCamera()->getProjectionMatrix()*Scene->getActiveCamera()->getViewMatrix()*Transformation());
		glm::vec4 prime = PVM * Center4;

		float length = glm::length(glm::vec3(prime.x, prime.y, prime.z));

		if (-prime.w < prime.x)
			in[0] ++;
		else
			out[0] ++;

		if (prime.w > prime.x)
			in[1] ++;
		else
			out[1] ++;

		if (-prime.w < prime.y)
			in[2] ++;
		else
			out[2] ++;

		if (prime.w > prime.y)
			in[3] ++;
		else
			out[3] ++;

		if (-prime.w < prime.z)
			in[4] ++;
		else
			out[4] ++;

		if (prime.w > prime.z)
			in[5] ++;
		else
			out[5] ++;
	}

	for (int i = 0; i < 6; ++ i) 
	{
		if (! in[i])
			return Outside;
		else if (out[i])
			return Inside;
	}
	return Inside;
}

bool const ISceneObject::isCullingEnabled() const
{
	return UseCulling;
}

void ISceneObject::setCullingEnabled(bool const culling)
{
	UseCulling = culling;
}
