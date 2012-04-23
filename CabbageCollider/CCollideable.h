#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include "../CabbageCore/SRect2.h"
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

class ICollisionResponder;

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

	SMaterial Material;

	SRect2 Area;

	CCollideable();

	float VisualDepth;


public:

	ICollisionResponder * CollisionResponder;
	virtual ~CCollideable();

	SRect2 const & getArea() const;
	void setArea(SRect2 const & area);

	SMaterial const & getMaterial() const;
	SMaterial & getMaterial();

	int CollideableType, CanCollideWith, CollideableLevel;

	virtual void draw();

	void setDepth(float);
	float getDepth();
   void setCollisionResponder( ICollisionResponder * collisionResponder );
};

class ICollisionResponder
{

public:

	virtual void OnCollision(CCollideable * Object) =0;

};

#endif
