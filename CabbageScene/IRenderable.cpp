#include "IRenderable.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


IRenderable::IRenderable()
    : Scale(1), DebugDataFlags(0)
{}


SVector3 const & IRenderable::getTranslation() const
{
    return Translation;
}

SVector3 const & IRenderable::getRotation() const
{
    return Rotation;
}

SVector3 const & IRenderable::getScale() const
{
    return Scale;
}


void IRenderable::setTranslation(SVector3 const & translation)
{
    Translation = translation;
}

void IRenderable::setRotation(SVector3 const & rotation)
{
    Rotation = rotation;
}

void IRenderable::setScale(SVector3 const & scale)
{
    Scale = scale;
}


void IRenderable::draw(CScene const * const scene)
{
}

SBoundingBox3 const & IRenderable::getBoundingBox() const
{
    return BoundingBox;
}

SBoundingBox3 & IRenderable::getBoundingBox()
{
    return BoundingBox;
}

bool const IRenderable::isDebugDataEnabled(EDebugData::Domain const type) const
{
    return (type & DebugDataFlags) != 0;
}

void IRenderable::enableDebugData(EDebugData::Domain const type)
{
    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags |= type;
}

void IRenderable::disableDebugData(EDebugData::Domain const type)
{
    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags ^= type;
}

bool const IRenderable::intersectsWithLine(SLine3 const & line) const
{
    return BoundingBox.intersectsWithLine(line);
}

bool const IRenderable::isVisible() const
{
    return Visible;
}

void IRenderable::setVisible(bool const isVisible)
{
    Visible = isVisible;
}
