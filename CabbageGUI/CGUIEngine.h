#ifndef _CABBAGEGUI_CGUIENGINE_H_INCLUDED_
#define _CABBAGEGUI_CGUIENGINE_H_INCLUDED_

#include <vector>

#include <SPosition2.h>

#include "CGUIWidget.h"


class CGUIEngine
{

	SPosition2 ScreenSize;
	std::vector<CGUIWidget *> Widgets;

public:

	CGUIEngine(SPosition2 const & screenSize);

	void addWidget(CGUIWidget * Widget);
	void removeWidget(CGUIWidget * Widget);
	void removeAllWidgets();

	void drawAll();

	void setScreenSize(SPosition2 const & screenSize);
	SPosition2 const getScreenSize() const;

};

#endif
