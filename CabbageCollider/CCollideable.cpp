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
	: TypeId(1), CollisionMask(-1), DetectionMask(-1), VisualDepth(0.f), GameplayElement(0)
{}

CCollideable::~CCollideable()
{}

SArea const & CCollideable::getInternalArea() const
{
	return Area;
}

SRect2<float> const CCollideable::getArea() const
{
	return SRect2<float>(Area.Position, Area.Size);
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

void CCollideable::setTypeId(int const type)
{
	TypeId = type;
}

void CCollideable::setCollisionMask(int const mask)
{
	CollisionMask = mask;
}

void CCollideable::setDetectionMask(int const mask)
{
	DetectionMask = mask;
}

int const CCollideable::getTypeId() const
{
	return TypeId;
}

int const CCollideable::getCollisionMask() const
{
	return CollisionMask;
}

int const CCollideable::getDetectionMask() const
{
	return DetectionMask;
}

bool const CCollideable::collidesWith(CCollideable * Object) const
{
	return Area.intersects(Object->getInternalArea());
}

bool const CCollideable::canCollideWith(CCollideable * Object) const
{
	return (TypeId & Object->CollisionMask) ||
		(Object->TypeId & CollisionMask);
}

bool const CCollideable::canDetectWith(CCollideable * Object) const
{
	return (TypeId & Object->DetectionMask) ||
		(Object->TypeId & DetectionMask);
}
