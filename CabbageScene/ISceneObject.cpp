#include "ISceneObject.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


ISceneObject::ISceneObject()
    : Scale(1), DebugDataFlags(0), UsesRotationMatrix(false), Visible(true)
{}


SVector3 const & ISceneObject::getTranslation() const
{
    return Translation;
}

SVector3 const & ISceneObject::getRotation() const
{
    return Rotation;
}

glm::mat4 const & ISceneObject::getRotationMatrix() const
{
    return RotationMatrix;
}

SVector3 const & ISceneObject::getScale() const
{
    return Scale;
}


void ISceneObject::setTranslation(SVector3 const & translation)
{
    Translation = translation;
}

void ISceneObject::setRotation(SVector3 const & rotation)
{
    UsesRotationMatrix = false;
    Rotation = rotation;
}

void ISceneObject::setRotation(glm::mat4 const & matrix)
{
    UsesRotationMatrix = true;
    RotationMatrix = matrix;
}

void ISceneObject::setScale(SVector3 const & scale)
{
    Scale = scale;
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
