#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include "CCollisionInteractor.h"

class CCollisionActor : public CCollisionInteractor
{

public:

	struct SAttributes : public CCollisionInteractor::SAttributes
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

	//! Behavior attributes of this actor
	SAttributes Attributes;

	//! Current movement action of this actor
	EActionType Action;

	//! While jumping, holds the current duration of the jump in seconds
	CollisionReal JumpTimer;

	//! True while this object is in the middle of a jump
	bool Jumping;
	bool WantsToJump;

	//! For debugging purposes, is true when the last update tick used an "allowed collision" movement
	bool AllowedMovement;


	//! Constructor
	friend class CCollisionEngine;
	CCollisionActor(CCollisionEngine * collisionEngine);


	//////////////////////////////////////
	// Update functions, used by engine //
	//////////////////////////////////////

	//! Updates the movement vectors of this actor
	//! Called once per tick
	//! Called first before initial update!
	virtual void updateVectors(CollisionReal const TickTime);

	//! Peforms a collision check between this actor and a collideable object
	//! Returns true if a standing event occured
	//! 
	virtual bool updateCollision(CCollideable * Object, float const TickTime);

	//! Performs a discrete collision check between this actor and a collideable object
	//! Called by updateCollision
	//! Returns the direction collisions that occured, if any
	//! Adjusts movement to prevent collision
	virtual int checkCollision(CCollideable * Object, CollisionReal const TickTime);

	//! Called by updateCollision when a downwards collision occurs
	virtual void onStanding(CCollideable * Object);

	//! Moves this object if a moveable Object caused a collision and push
	virtual void pushIfCollided(CCollisionObject * Object, SVec2 const Movement);

	//! After update, sends event responses for objects which left phase state
	virtual void updatePhaseList();

public:

	//! Destructor
	~CCollisionActor();

	//! See Attributes
	SAttributes const & getActorAttributes() const;
	//! See Attributes
	SAttributes & getActorAttributes();

	//! Sets the current movement action of this Actor
	void setAction(EActionType const & action);
	//! Gets the current movement action of this Actor
	EActionType const getAction() const;

	//! see Jumping
	bool const isJumping() const;
	//! Used to set jumping action
	void setJumping(bool const jumping);

	//! For debugging purposes, draws this actor onto the screen using FFP OpenGL
	virtual void draw();

};

#endif
