#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include <SRect2.h>
#include <vector>

#include "CCollideable.h"

class CCollisionObject;

class CCollisionActor : public CCollideable
{

public:

	struct SAttributes
	{
		//! Initial acceleration given during a jump
		CollisionReal JumpAccel;
		
		//! Duration in seconds that the actor can continuously jump for
		CollisionReal JumpLength;

		//! Maximum walking speed of this actor
		CollisionReal MaxWalk;

		//! Rate of acceleration for walking
		CollisionReal WalkAccel;

		//! Degree of movement control while falling (factors into max velocity)
		CollisionReal AirControl;

		//! Degree of movement control while falling (factors into acceleration)
		CollisionReal AirSpeedFactor;

		//! Rate at which horizontal velocity decreases while falling
		CollisionReal AirStandingFriction;

		//! Rate at which horizontal velocity decreases while falling
		CollisionReal GroundStandingFriction;

		//! Ratio of velocity reflected during collisions
		CollisionReal Bounce;

		//! Ratio of gravity affecting this object
		CollisionReal GravityMultiplier;

		//! Maximum step up for actor
		CollisionReal MaxStep;

		//! Default params ctor
		SAttributes();
	};

	//! Action type of this object, defines what movement the Actor is trying to perform
	class EActionType
	{

	public:

		enum Domain
		{
			None,
			MoveLeft,
			MoveRight
		};

	private:

		Domain Value;

	public:

		EActionType();
		EActionType(Domain const value);

		bool const operator == (Domain const value) const;

	};

protected:

	friend class CCollisionEngine;

	//! Current velocity of this actor
	SVec2 Velocity;

	//! While standing, points to the object being stood on
	CCollideable * Standing;

	//! Behavior attributes of this actor
	SAttributes Attributes;

	//! Current movement action of this actor
	EActionType Action;

	//! While jumping, holds the current duration of the jump in seconds
	CollisionReal JumpTimer;

	//! True while this object is in the middle of a jump
	bool Jumping;
	bool WantsToJump;

	//! Sets whether this actor should be accelerated by gravity
	bool GravityEnabled;

	//! Acceleration due to falling
	CollisionReal FallAcceleration;

	//! Impulse movement!
	std::vector<std::pair<SVec2, float> > Impulses;

	//! For debugging purposes, is true when the last update tick used an "allowed collision" movement
	bool AllowedMovement;

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
	//! Called first before initial update!
	void updateVectors(CollisionReal const TickTime);

	//! Peforms a collision check between this actor and a collideable object
	//! Returns true if a standing event occured
	//! 
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
