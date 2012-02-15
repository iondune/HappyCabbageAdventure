#include "ISceneObject.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


ISceneObject::ISceneObject()
    : Scale(1), DebugDataFlags(0), UsesRotationMatrix(false), Visible(true)
{}


void ISceneObject::setTranslation(SVector3 const & translation)
{
	Transformation.setTranslation(translation);
}

void ISceneObject::setRotation(SVector3 const & rotation)
{
	Transformation.setRotation(rotation);
}

void ISceneObject::setRotation(glm::mat4 const & matrix)
{
	Transformation.setRotation(matrix);
}

void ISceneObject::setScale(SVector3 const & scale)
{
	Transformation.setScale(scale);
}


void ISceneObject::draw(CScene const * const scene)
{
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

STransformation3 const & ISceneObject::getTransformation()
{
	return Transformation;
}
