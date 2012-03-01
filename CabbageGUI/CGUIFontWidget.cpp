#include "CGUIFontWidget.h"

#include <iostream>

//#define OGLFT_NO_SOLID
//#define OGLFT_NO_QT
#include "liboglft/OGLFT.h"

void CGUIFontWidget::makeRenderer(std::string const & FileName, float const Size)
{
	if (Renderer)
		delete Renderer;

	Renderer = new OGLFT::Translucent(FileName.c_str(), Size);

	if (Renderer == 0 || ! Renderer->isValid())
	{
		if (Renderer)
		{
			delete Renderer;
			Renderer = 0;
		}

		std::cerr << "Could not construct face from " << FileName << std::endl;
		return;
	}
	Renderer->setBackgroundColor(0.f, 0.f, 0.f, 0.f);
	Renderer->setHorizontalJustification(OGLFT::Face::CENTER);
	Renderer->setVerticalJustification(OGLFT::Face::MIDDLE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

CGUIFontWidget::CGUIFontWidget(std::string const & FileName, float const Size)
	: Renderer(0)
{
	makeRenderer(FileName, Size);
	setColor(SColor(0, 0, 0));
}

void CGUIFontWidget::draw()
{
	if (! Visible)
		return;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	Renderer->setStringRotation(Rotation);
	Renderer->draw(Position.X, Position.Y, Text.c_str());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void CGUIFontWidget::setColor(SColor const & Color)
{
	Renderer->setForegroundColor(Color.Red, Color.Green, Color.Blue, Color.Alpha);
}

SColor const CGUIFontWidget::getColor()
{
	return SColor(Renderer->foregroundRed(), Renderer->foregroundGreen(), Renderer->foregroundBlue(), Renderer->foregroundAlpha());
}

void CGUIFontWidget::setText(std::string const & text)
{
	Text = text;
}

std::string const & CGUIFontWidget::getText() const
{
	return Text;
}
