#include "CGUIImageWidget.h"

#include <CTextureLoader.h>

CGUIImageWidget::CGUIImageWidget(CImage * image, SVector2 const & size)
	: Size(size), Image(new CTexture(image))
{}

CGUIImageWidget::CGUIImageWidget(CTexture * image, SVector2 const & size)
	: Size(size), Image(image)
{}

CGUIImageWidget::CGUIImageWidget(std::string const & imagePath, SVector2 const & size)
	: Size(size), Image(CTextureLoader::loadTexture(imagePath))
{
}


void CGUIImageWidget::draw()
{
	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Image->getTextureHandle());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(Position.X, Position.Y, 0.f);
	glScalef(Size.X, Size.Y, 1.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(0, 0);

		glTexCoord2i(1, 0);
		glVertex2i(1, 0);

		glTexCoord2i(1, 1);
		glVertex2i(1, 1);

		glTexCoord2i(0, 1);
		glVertex2i(0, 1);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}


CTexture const * const CGUIImageWidget::getImage() const
{
	return Image;
}

void CGUIImageWidget::setImage(CTexture * image)
{
	Image = image;
}


SVector2 const & CGUIImageWidget::getSize() const
{
	return Size;
}

void CGUIImageWidget::setSize(SVector2 const & size)
{
	Size = size;
}
