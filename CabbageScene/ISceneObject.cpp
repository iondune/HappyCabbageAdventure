#include "ISceneObject.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


ISceneObject::ISceneObject()
	: DebugDataFlags(0), Visible(true), Parent(0), UseCulling(true), Immobile(false)
{}

void ISceneObject::updateAbsoluteTransformation()
{
	if (Immobile)
		return;

	AbsoluteTransformation = Transformation;
	if (Parent)
	{
		AbsoluteTransformation = Parent->AbsoluteTransformation* AbsoluteTransformation;
	}

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
	{
		(* it)->updateAbsoluteTransformation();
	}
}

glm::mat4 const & ISceneObject::getAbsoluteTransformation() const
{
	return AbsoluteTransformation;
}

void ISceneObject::setTranslation(SVector3 const & translation)
{
	if (Immobile)
		return;

	Translation = translation;
	Transformation.setTranslation(translation);
}

void ISceneObject::setRotation(SVector3 const & rotation)
{
	Rotation = rotation;
	Transformation.setRotation(rotation);
}

void ISceneObject::setRotation(glm::mat4 const & matrix)
{
	Transformation.setRotation(matrix);
}

void ISceneObject::setScale(SVector3 const & scale)
{
	//assert(!Immobile);
	Scale = scale;
	Transformation.setScale(scale);
}

void ISceneObject::update()
{
	if(Immobile)
		return;
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->update();
}

int timesCalled = 0, numObjects = 0, numCulled = 0;

void ISceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return;

	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it) {
		if (! (* it)->isCulled(scene)) {
			(* it)->draw(scene, Pass);
		}
		else {
			/*
			if((* it)->isImmobile())
			printf("Culled an immobile ISO\n");
			*/
			numCulled++;
		}
		numObjects++;
	}
}

SBoundingBox3 const & ISceneObject::getBoundingBox() const
{
	return BoundingBox;
}

SBoundingBox3 & ISceneObject::getBoundingBox()
{
	return BoundingBox;
}
void ISceneObject::setBoundingBox(SBoundingBox3 const & boundingBox) {
	BoundingBox = boundingBox;
}

bool const ISceneObject::isDebugDataEnabled(EDebugData::Domain const type) const
{
	return (type & DebugDataFlags) != 0;
}

void ISceneObject::enableDebugData(EDebugData::Domain const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->enableDebugData(type);

	if (type == EDebugData::None)
		DebugDataFlags = 0;
	else
		DebugDataFlags |= type;
}

void ISceneObject::disableDebugData(EDebugData::Domain const type)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->disableDebugData(type);

	if (type == EDebugData::None)
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

SVector3 const & ISceneObject::getRotation() const
{
	return Rotation;
}

SVector3 const & ISceneObject::getTranslation() const
{
	return Translation;
}

SVector3 const & ISceneObject::getScale() const
{
	return Scale;
}

#include "CCamera.h"

bool const ISceneObject::isCulled(CScene const * const Scene) const
{
	static bool const Inside = false;
	static bool const Outside = true;
	timesCalled++;

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
		SVector3 const Center = getBoundingBox().getCorner(i);
		glm::vec4 Center4(Center.X, Center.Y, Center.Z, 1.f);

		glm::mat4 PVM;
		if(!Immobile)
			PVM = (Scene->getActiveCamera()->getProjectionMatrix()*Scene->getActiveCamera()->getViewMatrix()*Transformation());
		else
			PVM = Scene->getActiveCamera()->getProjectionMatrix()*Scene->getActiveCamera()->getViewMatrix()*glm::mat4(1.);
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

bool ISceneObject::isImmobile() {
	return Immobile;
}
void ISceneObject::setImmobile(bool val) {
	Immobile = val;
}

SVector3 ISceneObject::getWorldBoundingBoxMinPoint() {
	if(!Immobile) {
		SVector3 p = getBoundingBox().MinCorner; 
		glm::vec4 p4(p.X, p.Y, p.Z, 1.f);
		glm::vec4 temp = Transformation() * p4; 

		//printf("Min: %0.2f %0.2f %0.2f\n", temp.x, temp.y, temp.z);
		return SVector3(temp.x, temp.y, temp.z);
	}
	else
		return getBoundingBox().MinCorner;
}

SBoundingBox3 ISceneObject::getWorldBoundingBox() {
	if(!Immobile) {
		SVector3 min = getBoundingBox().MinCorner; 
		glm::vec4 min4(min.X, min.Y, min.Z, 1.f);
		glm::vec4 temp = Transformation() * min4; 
		//printf("Min: %0.2f %0.2f %0.2f\n", temp.x, temp.y, temp.z);

		SVector3 max = getBoundingBox().MaxCorner; 
		glm::vec4 max4(max.X, max.Y, max.Z, 1.f);
		glm::vec4 temp2 = Transformation() * max4; 
		//printf("Max: %0.2f %0.2f %0.2f\n", temp2.x, temp2.y, temp2.z);

		return SBoundingBox3(SVector3(temp.x, temp.y, temp.z), SVector3(temp2.x, temp2.y, temp2.z));
	}
	else
		return getBoundingBox();
}

bool sortByZTranslation(ISceneObject *a, ISceneObject *b) {
   return a->getTranslation().Z > b->getTranslation().Z;
}

void ISceneObject::sortChildrenByZTranslation() {
   Children.sort(sortByZTranslation);
}

bool const ISceneObject::isCullingEnabled() const
{
	return UseCulling;
}

void ISceneObject::setCullingEnabled(bool const culling)
{
	UseCulling = culling;
}

void ISceneObject::load(CScene const * const Scene, ERenderPass const Pass)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->load(Scene, Pass);
}

void ISceneObject::setTreeImmobile(bool value) {
	Immobile = value;
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->setTreeImmobile(value);
}

int ISceneObject::getNumLeaves() {
	if(Children.size() == 0)
		return 1;
	int toRet = 0;
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it) {
		toRet += (* it)->getNumLeaves();
	}
	return toRet;
}
