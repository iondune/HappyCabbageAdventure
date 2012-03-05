#include "CGUIEngine.h"

#include <algorithm>

#ifdef __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif


CGUIEventReceiver::CGUIEventReceiver(CGUIEngine * engine)
	: Engine(engine)
{
	Engine->getEventManager()->OnWidgetClick.connect(this, & CGUIEventReceiver::OnWidgetClick);
	Engine->getEventManager()->OnWidgetHover.connect(this, & CGUIEventReceiver::OnWidgetHover);
	Engine->getEventManager()->OnWidgetUnHover.connect(this, & CGUIEventReceiver::OnWidgetUnHover);
}

CGUIEventReceiver::~CGUIEventReceiver()
{
	Engine->getEventManager()->OnWidgetClick.disconnect(this);
	Engine->getEventManager()->OnWidgetHover.disconnect(this);
	Engine->getEventManager()->OnWidgetUnHover.disconnect(this);
}

void CGUIEventReceiver::OnWidgetHover(CGUIWidget * Widget)
{}

void CGUIEventReceiver::OnWidgetUnHover(CGUIWidget * Widget)
{}

void CGUIEventReceiver::OnWidgetClick(CGUIWidget * Widget)
{}


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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ScreenSize.X / (float) ScreenSize.Y, 0, 1, -1, 1);

	for (std::vector<CGUIWidget *>::iterator it = Widgets.begin(); it != Widgets.end(); ++ it)
		(* it)->draw();
}

CGUIEventManager * const CGUIEngine::getEventManager()
{
	return & EventManager;
}

CGUIEventManager const * const CGUIEngine::getEventManager() const
{
	return & EventManager;
}

void CGUIEngine::OnMouseEvent(SMouseEvent const & Event)
{
	SVector2 RelativeLocation = Event.RelativeLocation;
	RelativeLocation.X *= ScreenSize.X / (float) ScreenSize.Y;
	RelativeLocation.Y = 1.f - RelativeLocation.Y;

	switch (Event.Type.Value)
	{
	case SMouseEvent::EType::Click:

		if (! Event.Pressed && Event.Button == SMouseEvent::EButton::Left)
		{
			for (std::vector<CGUIWidget *>::iterator it = Widgets.begin(); it != Widgets.end(); ++ it)
				if ((* it)->getBoundingBox().isPointInside(RelativeLocation))
					EventManager.OnWidgetClick(* it);
		}

		break;

	case SMouseEvent::EType::Move:

		for (std::vector<CGUIWidget *>::iterator it = Widgets.begin(); it != Widgets.end(); ++ it)
		{
			if ((* it)->getBoundingBox().isPointInside(RelativeLocation))
			{
				if (! (* it)->isHovered())
				{
					EventManager.OnWidgetHover(* it);
					(* it)->Hovered = true;
				}
			}
			else if ((* it)->isHovered())
			{
				EventManager.OnWidgetUnHover(* it);
				(* it)->Hovered = false;
			}
		}

		break;
	}
}
