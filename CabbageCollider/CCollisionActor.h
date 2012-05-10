#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include <SRect2.h>

#include "CCollisionObject.h"

class ICollisionResponder;

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

		//! ?
		int Reacts;

		//! Default params ctor
		SAttributes();
	};

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

		bool const operator == (Domain const value);

	};

protected:

	friend class CCollisionEngine;

	//! ?
	CollisionReal FallAcceleration;

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

	//! ?
	bool ControlFall;

	CCollisionActor();

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

	//! Impulse movement!
	std::vector<std::pair<SVec2, float> > Impulses;

	//! For debugging purposes, is true when the last update tick used an "allowed collision" movement
	bool AllowedMovement;

	//! Position held by this actor during last tick
	SVec2 LastPosition;

	//! Movement of this actor during current tick
	SVec2 Movement;

public:

	//! Destructor
	~CCollisionActor();


	//! ?
	float Gravity;

	//! Checks if this actor is directly above an object, and if so, returns the height between them
	bool isAbove(CCollisionObject * Object, float & height) const;

	//! See Standing
	bool const isStanding() const;

	//! See Velocity
	void setVelocity(SVec2 const & vel);
	//! See Velocity
	SVec2 const & getVelocity() const;

	//! see Attributes
	SAttributes const & getAttributes() const;
	//! see Attributes
	SAttributes & getAttributes();

	// Action control
	void setAction(EActionType const & action);
	EActionType const getAction() const;

	//! see Jumping
	bool const isJumping() const;
	//! Used to set jumping action
	void setJumping(bool const jumping);

	void setControlFall(bool const fall);
	bool getControlFall();

	//! see FallAcceleration
	void setFallAcceleration(CollisionReal speed);
	//! see FallAcceleration
	float getFallAcceleration();

	//! For debugging purposes, draws this actor onto the screen using FFP OpenGL
	virtual void draw();

	void addImpulse(SVec2 const & velocity, float const Duration);

};

#endif
