#include "CCollideable.h"

#ifdef __unix__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif


CCollideable::SMaterial::SMaterial()
	: Friction(1.f), Elasticity(0.f)
{}

CCollideable::CCollideable()
	: CollisionType(1), CollisionMask(-1), VisualDepth(0.f)
{}

CCollideable::~CCollideable()
{}

SArea const & CCollideable::getArea() const
{
	return Area;
}

void CCollideable::setArea(SArea const & area)
{
	Area = area;
}

CCollideable::SMaterial const & CCollideable::getMaterial() const
{
	return Material;
}

CCollideable::SMaterial & CCollideable::getMaterial()
{
	return Material;
}

void CCollideable::draw()
{
	glPushMatrix();
	glTranslatef((float) Area.Position.X, (float) Area.Position.Y, 0);

	glBegin(GL_QUADS);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f((float) Area.Size.X, 0.f, 0.f);
	glVertex3f((float) Area.Size.X, (float) Area.Size.Y, 0.f);
	glVertex3f(0.f, (float) Area.Size.Y, 0.f);
	glEnd();

	glPopMatrix();
}

void CCollideable::setVisualDepth(float const f)
{
	VisualDepth = f;
}

float const CCollideable::getVisualDepth() const
{
	return VisualDepth;
}

void CCollideable::setCollisionType(unsigned int const type)
{
	CollisionType = type;
}

void CCollideable::setCollisionMask(unsigned int const mask)
{
	CollisionMask = mask;
}

int const CCollideable::getCollisionType() const
{
	return CollisionType;
}

int const CCollideable::getCollisionMask() const
{
	return CollisionMask;
}

bool const CCollideable::collidesWith(CCollideable * Object) const
{
	return Area.intersects(Object->getArea());
}
