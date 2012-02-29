#include "CGUIEngine.h"

#include <algorithm>

#ifdef __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif

CGUIEngine::CGUIEngine(SPosition2 const & screenSize)
	: ScreenSize(screenSize)
{}

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
	glViewport(0, 0, ScreenSize.X, ScreenSize.Y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float) ScreenSize.X, 0, (float) ScreenSize.Y, -1, 1);

	for (std::vector<CGUIWidget *>::iterator it = Widgets.begin(); it != Widgets.end(); ++ it)
		(* it)->draw();
}

void CGUIEngine::setScreenSize(SPosition2 const & screenSize)
{
	ScreenSize = screenSize;
}

SPosition2 const CGUIEngine::getScreenSize() const
{
	return ScreenSize;
}
