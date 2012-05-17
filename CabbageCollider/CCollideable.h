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


class CCollideable;

class ECollisionType
{

public:

	enum Domain
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8,
		Responded = 16
	};

	Domain Value;

};

struct SCollisionEvent
{
	CCollideable * This;
	CCollideable * Other;

	ECollisionType::Domain Direction;
};

class CGameplayElement;

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

	CGameplayElement * GameplayElement;

public:

	virtual ~CCollideable();

	SArea const & getInternalArea() const;
	SRect2<float> const getArea() const;

	template <typename T>
	void setArea(SRect2<T> const & area)
	{
		Area = SArea(area.Position, area.Size);
	}

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

	CGameplayElement * getGameplayElement()
	{
		return GameplayElement;
	}

	void setGameplayElement(CGameplayElement * Element)
	{
		GameplayElement = Element;
	}

	sigslot::signal1<SCollisionEvent const &> OnCollision;

};

#endif
