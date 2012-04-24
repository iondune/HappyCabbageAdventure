#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include <SRect2.h>

#include "CCollisionObject.h"

class ICollisionResponder;

class ECollisionType
{

public:

	enum Domain
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8
	};

	Domain Value;

};

class CCollisionActor : public CCollideable
{

public:

	struct SAttributes
	{
		//! Initial acceleration given during a jump
		float JumpAccel;
		
		//! Duration in seconds that the actor can continuously jump for
		float JumpLength;

		//! Maximum walking speed of this actor
		float MaxWalk;

		//! Rate of acceleration for walking
		float WalkAccel;

		//! Degree of movement control while falling (factors into max velocity)
		float AirControl;

		//! Degree of movement control while falling (factors into acceleration)
		float AirSpeedFactor;

		//! Rate at which horizontal velocity decreases while falling
		float AirStandingFriction;

		//! Rate at which horizontal velocity decreases while falling
		float GroundStandingFriction;

		//! Ratio of velocity reflected during collisions
		float Bounce;

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
	float FallAcceleration;

	//! Current velocity of this actor
	SVector2f Velocity;

	//! While standing, points to the object being stood on
	CCollideable * Standing;


	//! Behavior attributes of this actor
	SAttributes Attributes;

	//! Current movement action of this actor
	EActionType Action;

	//! While jumping, holds the current duration of the jump in seconds
	float JumpTimer;

	//! True while this object is in the middle of a jump
	bool Jumping;

	//! ?
	bool ControlFall;

	CCollisionActor();

	// Update functions, used by engine
	void updateVectors(float const TickTime);

	bool updateCollision(CCollideable * Object, float const TickTime);
	void onStanding(CCollideable * Object);

	void pushIfCollided(CCollisionObject * Object, SVector2f const & Movement);

	bool Impulse;
	float ImpulseTimer;
	SVector2f ImpulseVelocity;

	bool AllowedMovement;

public:

	~CCollisionActor();

	SVector2f LastPosition, Movement;
	int checkCollision(CCollideable * Object, float const TickTime);
	float Gravity;
	bool collidesWith(CCollisionObject * Object) const;
	bool isAbove(CCollisionObject * Object, float & height) const;

	//! See Standing
	bool const isStanding() const;

	//! See Velocity
	void setVelocity(SVector2f const & vel);
	//! See Velocity
	SVector2f const & getVelocity() const;

	//! see Attributes
	SAttributes const & getAttributes() const;
	//! see Attributes
	SAttributes & getAttributes();

	// Action control
	void setAction(EActionType const & action);
	EActionType getAction();

	//! see Jumping
	bool isJumping();
	//! Used to set jumping action
	void setJumping(bool const jumping);

	void setControlFall(bool const fall);
	bool getControlFall();

	//! see FallAcceleration
	void setFallAcceleration(float speed);
	//! see FallAcceleration
	float getFallAcceleration();

	virtual void draw();

	void setImpulse(SVector2f const & velocity, float const duration = 0.3f);
	void addImpulse(SVector2f const & velocity);

};


#endif
