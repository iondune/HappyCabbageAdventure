#include "IRenderable.h"


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
