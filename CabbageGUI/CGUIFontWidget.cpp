#include "CGUIFontWidget.h"

#include <iostream>

//#define OGLFT_NO_SOLID
//#define OGLFT_NO_QT
#include "liboglft\OGLFT.h"

#include <CApplication.h>

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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

CGUIFontWidget::CGUIFontWidget(std::string const & FileName, float const Size)
{
	makeRenderer(FileName, Size);
	
}

void CGUIFontWidget::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glViewport(0, 0, CApplication::get().getWindowSize().X, CApplication::get().getWindowSize().Y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1.f, 0, 1.f, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	Renderer->draw(Position.X, Position.Y, Text.c_str());
	glDisable(GL_DEPTH_TEST);
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
