#ifndef _CABBAGEGUI_CGUIWIDGET_H_INCLUDED_
#define _CABBAGEGUI_CGUIWIDGET_H_INCLUDED_

#include <SVector2.h>

class CGUIWidget
{

protected:

	SVector2 Position;
	float Rotation;

	bool Visible;

public:

	CGUIWidget();

	SVector2 const & getPosition() const;
	void setPosition(SVector2 const & position);

	float const getRotation() const;
	void setRotation(float  const rotation);

	virtual void draw();

	bool const isVisible() const;
	void setVisible(bool const visible);

};

#endif
