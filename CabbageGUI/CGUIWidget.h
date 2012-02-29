#ifndef _CABBAGEGUI_CGUIWIDGET_H_INCLUDED_
#define _CABBAGEGUI_CGUIWIDGET_H_INCLUDED_

#include <SVector2.h>

class CGUIWidget
{

protected:

	SVector2 Position;

public:

	SVector2 const & getPosition() const;
	void setPosition(SVector2 const & position);

	virtual void draw();

};

#endif
