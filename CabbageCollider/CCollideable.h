#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include <SRect2.h>
#include <sigslot/sigslot.h>


// Poor man's enum that doesn't belong here
#define INTERACTOR_ALL 31
#define INTERACTOR_BLOCKS 1
#define INTERACTOR_ITEMS 2
#define INTERACTOR_ACTORS 4
#define INTERACTOR_SUPERACTORS 8
#define INTERACTOR_NONCOLLIDERS 16
#define INTERACTOR_SUPERNONCOLLIDERS 32
#define INTERACTOR_ALL_ALL -1
#define INTERACTOR_NON 128

// Collision types
typedef double CollisionReal;
typedef SVector2<CollisionReal> SVec2;
typedef SRect2<CollisionReal> SArea;


class CCollideable;

//! Enumeration for different collision types, used by SCollisionEvent
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
		Responded = 16,
		UnPhase = 32
	};

	Domain Value;

};

//! Event emitted upon collision
struct SCollisionEvent
{
	CCollideable * This;
	CCollideable * Other;

	ECollisionType::Domain Direction;
};

class CGameplayElement;
class CCollisionEngine;

//! Base collision object
class CCollideable
{

public:

	//! Controllable material parameters
	struct SMaterial
	{
		float Friction;
		float Elasticity;

		SMaterial();
	};

protected:

	CCollideable(CCollisionEngine * collisionEngine);

	//! Material properties
	SMaterial Material;
	
	//! Position of this collideable
	SArea Area;

	float VisualDepth;

	//! Binary identifier for this object
	int TypeId;

	//! Bitfield for determining which objects this object will collide with
	int CollisionMask;
	
	//! Bitfield for determining which objects this object can detect an intersection with
	int DetectionMask;

	//! Associate a GameplayElement with this object
	CGameplayElement * GameplayElement;

	//! Keep reference to the encapsulating CollisionEngine
	CCollisionEngine * CollisionEngine;

public:

	virtual ~CCollideable();

	//! Returns the engine-internal Area (positions are represented as doubles within the engine)
	SArea const & getInternalArea() const;

	//! Returns the floating point Area of this object
	SRect2<float> const getArea() const;

	//! Area-set method for using either float or double Areas
	virtual void setArea(SRect2f const & area);
	virtual void setArea(SRect2d const & area);

	//! Used to check the Material parameters of this object
	SMaterial const & getMaterial() const;

	//! Used to modify the Material parameters of this object
	SMaterial & getMaterial();

	//! Used for debug-drawing this object using FFP OpenGL
	virtual void draw();


	//! Sets the visual depth of this object, external use
	void setVisualDepth(float const depth);

	//! Gets the visual depth of this object, external use
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

	CGameplayElement * getGameplayElement();
	CGameplayElement const * getGameplayElement() const;
	void setGameplayElement(CGameplayElement * Element);

	CCollisionEngine * getCollisionEngine();
	CCollisionEngine const * getCollisionEngine() const;
	void setCollisionEngine(CCollisionEngine * collisionEngine);

	sigslot::signal1<SCollisionEvent const &> OnCollision;
	sigslot::signal1<SCollisionEvent const &> OnPhaseBegin;
	sigslot::signal1<SCollisionEvent const &> OnPhaseEnd;

};

#endif
