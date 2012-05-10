#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include <SRect2.h>
#include <sigslot/sigslot.h>

#define INTERACTOR_ALL 31
#define INTERACTOR_BLOCKS 1
#define INTERACTOR_ITEMS 2
#define INTERACTOR_ACTORS 4
#define INTERACTOR_SUPERACTORS 8
#define INTERACTOR_NONCOLLIDERS 16
#define INTERACTOR_SUPERNONCOLLIDERS 32
#define INTERACTOR_ALL_ALL -1

typedef double CollisionReal;
typedef SVector2<CollisionReal> SVec2;
typedef SRect2<CollisionReal> SArea;

class CCollideable
{

public:

	struct SMaterial
	{
		float Friction;
		float Elasticity;

		SMaterial();
	};

protected:

	friend class CCollisionEngine;
	CCollideable();

	SMaterial Material;
	SArea Area;

	float VisualDepth;

	int TypeId;

	int CollisionMask;
	int DetectionMask;

public:

	virtual ~CCollideable();

	SArea const & getArea() const;
	void setArea(SArea const & area);

	SMaterial const & getMaterial() const;
	SMaterial & getMaterial();

	virtual void draw();

	void setVisualDepth(float const depth);
	float const getVisualDepth() const;

	void setTypeId(int const type);
	void setCollisionMask(int const mask);
	void setDetectionMask(int const mask);

	int const getTypeId() const;
	int const getCollisionMask() const;
	int const getDetectionMask() const;

	bool const collidesWith(CCollideable * Object) const;
	bool const canCollideWith(CCollideable * Object) const;
	bool const canDetectWith(CCollideable * Object) const;

};

#endif
