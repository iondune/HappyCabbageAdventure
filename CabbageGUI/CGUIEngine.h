#ifndef _CABBAGEGUI_CGUIENGINE_H_INCLUDED_
#define _CABBAGEGUI_CGUIENGINE_H_INCLUDED_

#include <vector>

#include <SPosition2.h>

#include "CGUIWidget.h"


class CGUIEngine
{

	std::vector<CGUIWidget *> Widgets;

	SPosition2 const ScreenSize;

public:

	CGUIEngine(SPosition2 const & screenSize);

	void addWidget(CGUIWidget * Widget);
	void removeWidget(CGUIWidget * Widget);
	void removeAllWidgets();

	void drawAll();

};

#endif
