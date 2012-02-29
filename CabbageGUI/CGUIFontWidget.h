#ifndef _CABBAGEGUI_CFONT_H_INCLUDED_
#define _CABBAGEGUI_CFONT_H_INCLUDED_

#include <string>

#include <SColor.h>

#include "CGUIWidget.h"

namespace OGLFT
{
	class Face;
}

class CGUIFontWidget : public CGUIWidget
{

	void makeRenderer(std::string const & FileName, float const Size);
	
	OGLFT::Face * Renderer;
	std::string Text;

public:

	CGUIFontWidget(std::string const & FileName, float const Size);

	void draw();

	void setColor(SColor const & Color);
	SColor const getColor();

	void setText(std::string const & text);
	std::string const & getText() const;

};

#endif
