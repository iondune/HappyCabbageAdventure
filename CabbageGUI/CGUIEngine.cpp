#include "CGUIEngine.h"

#include <algorithm>

void CGUIEngine::addWidget(CGUIWidget * Widget)
{
	Widgets.push_back(Widget);
}

void CGUIEngine::removeWidget(CGUIWidget * Widget)
{
	Widgets.erase(std::remove(Widgets.begin(), Widgets.end(), Widget), Widgets.end());
}

void CGUIEngine::removeAllWidgets()
{
	Widgets.clear();
}

void CGUIEngine::drawAll()
{
	for (std::vector<CGUIWidget *>::iterator it = Widgets.begin(); it != Widgets.end(); ++ it)
		(* it)->draw();
}

void CGUIEngine::setScreenSize(SPosition2 screenSize)
{
	ScreenSize = screenSize;
}

SPosition2 const CGUIEngine::getScreenSize() const
{
	return ScreenSize;
}
