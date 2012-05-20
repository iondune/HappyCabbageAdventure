#ifdef _CABBAGECOLLIDER_CINTERATOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CINTERATOR_H_INCLUDED_

#include <SRect2.h>
#include <vector>

#include "CCollideable.h"

class CCollisionActor : public CCollideable
{

public:

	struct SAttributes
	{

		//! Ratio of velocity reflected during collisions
		CollisionReal Bounce;

		//! Default params ctor
		SAttributes();

	};

protected:

	friend class CCollisionEngine;

	//! Current velocity of this actor
	SVec2 Velocity;

	//! While standing, points to the object being stood on
	CCollideable * Standing;

	//! Behavior attributes of this actor
	SAttributes Attributes;

	//! Sets whether this actor should be accelerated by gravity
	bool GravityEnabled;

	//! Acceleration due to falling
	CollisionReal FallAcceleration;

	//! Impulse movement!
	std::vector<std::pair<SVec2, float> > Impulses;

	//! Position held by this actor during last tick
	SVec2 LastPosition;

	//! Movement of this actor during current tick
	SVec2 Movement;

	//! Lists maintaining which objects are currently phased through this object
	std::set<CCollideable *> PhaseList, NewPhaseList;


	//! Constructor
	CCollisionActor(CCollisionEngine * collisionEngine);


	//////////////////////////////////////
	// Update functions, used by engine //
	//////////////////////////////////////

	//! Updates the movement vectors of this actor
	//! Called once per tick
	void updateVectors(CollisionReal const TickTime);

	//! Peforms a collision check between this actor and a collideable object
	//! Returns true if a standing event occured
	bool updateCollision(CCollideable * Object, float const TickTime);

	//! Performs a discrete collision check between this actor and a collideable object
	//! Called by updateCollision
	//! Returns the direction collisions that occured, if any
	//! Adjusts movement to prevent collision
	int checkCollision(CCollideable * Object, CollisionReal const TickTime);

	//! Called by updateCollision when a downwards collision occurs
	void onStanding(CCollideable * Object);

	//! Moves this object if a moveable Object caused a collision and push
	void pushIfCollided(CCollisionObject * Object, SVec2 const Movement);

	//! After update, sends event responses for objects which left phase state
	void updatePhaseList();

public:

	//! Destructor
	~CCollisionActor();

	//! Checks if this actor is directly above an object, and if so, returns the height between them
	bool isAbove(CCollisionObject * Object, float & height) const;

	//! See Standing
	bool const isStanding() const;

	//! See Velocity
	void setVelocity(SVec2 const & vel);
	//! See Velocity
	SVec2 const & getVelocity() const;

	//! See Attributes
	SAttributes const & getAttributes() const;
	//! See Attributes
	SAttributes & getAttributes();

	//! Sets the current movement action of this Actor
	void setAction(EActionType const & action);
	//! Gets the current movement action of this Actor
	EActionType const getAction() const;

	//! see Jumping
	bool const isJumping() const;
	//! Used to set jumping action
	void setJumping(bool const jumping);

	//! Sets whether this object is affected by Gravity
	void setGravityEnabled(bool const gravityEnabled);
	//! Gets if this object is affected by Gravity
	bool const isGravityEnabled() const;

	//! For debugging purposes, draws this actor onto the screen using FFP OpenGL
	virtual void draw();

	//! Adds an impulse to this Actor
	void addImpulse(SVec2 const & velocity, float const Duration = 0.001f);

};

#endif
