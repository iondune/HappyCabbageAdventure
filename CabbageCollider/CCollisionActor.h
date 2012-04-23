#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include "../CabbageCore/SRect2.h"

#include "CCollisionObject.h"

class ICollisionResponder;

class ECollisionType
{

public:

	enum Value
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8
	};

};

class CCollisionActor : public CCollideable
{

public:

	struct SAttributes
	{
		float JumpAccel;
		float JumpLength;
		float MaxWalk;
		float WalkAccel;
		float AirControl;
		float AirSpeedFactor;

		float AirStandingFriction;
		float GroundStandingFriction;

		float Bounce;
		int Reacts;

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

	float FallAcceleration;
	SVector2 Velocity;
	CCollideable * Standing;


	SAttributes Attributes;
	EActionType Action;
	float JumpTimer;
	bool Jumping, ControlFall;

	CCollisionActor();

	// Update functions, used by engine
	void updateVectors(float const TickTime);

	bool updateCollision(CCollideable * Object, float const TickTime);
	void onStanding(CCollideable * Object);

	void pushIfCollided(CCollisionObject * Object, SVector2 const & Movement);

	bool Impulse;
	float ImpulseTimer;
	SVector2 ImpulseVelocity;

public:

	~CCollisionActor();

	SVector2 LastPosition, Movement;
	int checkCollision(CCollideable * Object, float const TickTime);
	int ignoreCollision(CCollideable * Object, float const TickTime);
	float Gravity;
	bool collidesWith(CCollisionObject * Object) const;
	bool isAbove(CCollisionObject * Object, float & height) const;

	bool const isStanding() const;

	void setVelocity(SVector2 const & vel);
	SVector2 const & getVelocity() const;

	SAttributes const & getAttributes() const;
	SAttributes & getAttributes();

	// Action control
	void setAction(EActionType const & action);
	EActionType getAction();
	bool isJumping();
	void setJumping(bool const jumping);
	void setControlFall(bool const fall);
	bool getControlFall();
	void setStanding(bool);
	void setFallAcceleration(float speed);
	float getFallAcceleration();

	virtual void draw();

	void setImpulse(SVector2 const & velocity, float const duration = 0.3f);
	void addImpulse(SVector2 const & velocity);

};


#endif
