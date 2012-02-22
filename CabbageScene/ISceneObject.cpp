#include "ISceneObject.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


ISceneObject::ISceneObject()
    : DebugDataFlags(0), Visible(true), Parent(0)
{}


void ISceneObject::updateAbsoluteTransformation()
{
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
	Scale = scale;
	Transformation.setScale(scale);
}


void ISceneObject::update()
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->update();
}

void ISceneObject::draw(CScene const * const scene)
{
	for (std::list<ISceneObject *>::iterator it = Children.begin(); it != Children.end(); ++ it)
		(* it)->draw(scene);
}

SBoundingBox3 const & ISceneObject::getBoundingBox() const
{
    return BoundingBox;
}

SBoundingBox3 & ISceneObject::getBoundingBox()
{
    return BoundingBox;
}

bool const ISceneObject::isDebugDataEnabled(EDebugData::Domain const type) const
{
    return (type & DebugDataFlags) != 0;
}

void ISceneObject::enableDebugData(EDebugData::Domain const type)
{
    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags |= type;
}

void ISceneObject::disableDebugData(EDebugData::Domain const type)
{
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
