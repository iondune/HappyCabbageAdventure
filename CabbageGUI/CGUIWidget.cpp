#include "CGUIWidget.h"

CGUIWidget::CGUIWidget()
	: Rotation(0.f)
{}

SVector2 const & CGUIWidget::getPosition() const
{
	return Position;
}

void CGUIWidget::setPosition(SVector2 const & position)
{
	Position = position;
}

float const CGUIWidget::getRotation() const
{
	return Rotation;
}

void CGUIWidget::setRotation(float  const rotation)
{
	Rotation = rotation;
}

void CGUIWidget::draw()
{}
