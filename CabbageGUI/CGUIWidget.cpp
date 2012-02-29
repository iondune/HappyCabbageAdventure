#include "CGUIWidget.h"

SVector2 const & CGUIWidget::getPosition() const
{
	return Position;
}

void CGUIWidget::setPosition(SVector2 const & position)
{
	Position = position;
}

void CGUIWidget::draw()
{
}
