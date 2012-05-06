#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include <SRect2.h>

#define COLLIDEABLE_TYPE_BLOCK 10
#define COLLIDEABLE_TYPE_ELEVATOR 11
#define COLLIDEABLE_TYPE_ACTOR 12
#define COLLIDEABLE_TYPE_APPLE 13
#define COLLIDEABLE_TYPE_ORANGE 14
#define COLLIDEABLE_TYPE_KIWI 15
#define COLLIDEABLE_TYPE_FLAME 16
#define COLLIDEABLE_TYPE_PKIWI 17
#define COLLIDEABLE_TYPE_GRAPE 18
#define COLLIDEABLE_TYPE_PGRAPE 19

#define COLLIDEABLE_TYPE_ITEM 100 
#define COLLIDEABLE_TYPE_HEALTH 101 
#define COLLIDEABLE_TYPE_ENERGY 102 

#define COLLIDEABLE_TYPE_PLAYER 500
#define COLLIDEABLE_TYPE_COLLIDEABLE -1 

#define INTERACTOR_ALL 31 
#define INTERACTOR_BLOCKS 1
#define INTERACTOR_ITEMS 2
#define INTERACTOR_ACTORS 4
#define INTERACTOR_SUPERACTORS 8
#define INTERACTOR_NONCOLLIDERS 16
#define INTERACTOR_SUPERNONCOLLIDERS 32 
#define INTERACTOR_NULL_BLOCK 64 
#define INTERACTOR_ALL_ALL 1+2+4+8+16+32+64

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
