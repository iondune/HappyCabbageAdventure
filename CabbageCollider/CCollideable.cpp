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

CCollideable::CCollideable() : CollideableType(COLLIDEABLE_TYPE_COLLIDEABLE), CollideableLevel(INTERACTOR_ALL), VisualDepth(0.0f), CollisionResponder(NULL), Element(NULL)
{}

CCollideable::~CCollideable()
{}

void CCollideable::setElement(CGameplayElement * e) {
   Element = e;
}

CGameplayElement &CCollideable::getElement() {
   return *Element;
}

SRect2 const & CCollideable::getArea() const
{
	return Area;
}

void CCollideable::setArea(SRect2 const & area)
{
	Area = area;
}

CCollideable::SMaterial const & CCollideable::getMaterial() const
{
	return Material;
}

void CCollideable::setDepth(float f) {
	VisualDepth = f;
}
float CCollideable::getDepth() {
	return VisualDepth;
}


CCollideable::SMaterial & CCollideable::getMaterial()
{
	return Material;
}

void CCollideable::draw()
{
	glPushMatrix();
	glTranslatef(Area.Position.X, Area.Position.Y, 0);

	glBegin(GL_QUADS);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(Area.Size.X, 0.f, 0.f);
	glVertex3f(Area.Size.X, Area.Size.Y, 0.f);
	glVertex3f(0.f, Area.Size.Y, 0.f);
	glEnd();

	glPopMatrix();
}

void CCollideable::setCollisionResponder( ICollisionResponder * collisionResponder )
{
	CollisionResponder = collisionResponder;
}
