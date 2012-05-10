#include "CCollisionActor.h"

#include <algorithm>

#include "../CabbageCore/Utils.h"
#include "CCollisionEngine.h"

#ifdef __unix__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif


CCollisionActor::SAttributes::SAttributes()
	: MaxWalk(4.5f),
	WalkAccel(60.f),
	JumpAccel(6.4f),
	JumpLength(0.6f),
	AirControl(0.75f),
	AirSpeedFactor(0.7f),
	AirStandingFriction(0.99f),
	GroundStandingFriction(0.95f),
	Reacts(1),
	Bounce(0.0f)
{}


CCollisionActor::EActionType::EActionType()
	: Value(None)
{}

CCollisionActor::EActionType::EActionType(Domain const value)
	: Value(value)
{}

bool const CCollisionActor::EActionType::operator == (Domain const value)
{
	return Value == value;
}


CCollisionActor::CCollisionActor()
	: Standing(0), JumpTimer(0.f), FallAcceleration(0), ControlFall(true) 
{
	TypeId = INTERACTOR_ACTORS;
	CollisionMask = INTERACTOR_BLOCKS | INTERACTOR_ACTORS;

	Gravity = (CollisionReal) 100.0;

	AllowedMovement = false;
}

CCollisionActor::~CCollisionActor()
{}

int CCollisionActor::checkCollision(CCollideable * Object, CollisionReal const TickTime)
{
	int Out = ECollisionType::None;

	if (canCollideWith(Object))
	{
		Area.Position = LastPosition;

		for (int i = 0; i < 2; ++ i)
		{
			CollisionReal OriginalMovement = Movement[i];

			if (equals(Movement[i], (CollisionReal) 0.0))
				continue;

			Area.Position[i] = LastPosition[i] + Movement[i];

			if (Area.intersects(Object->getInternalArea()))
			{
				if (Movement[i] > (CollisionReal) 0.0)
				{
					//TryMovement = 0.f;//std::max(Object->getArea().Position[i] - (LastPosition + Area.Size)[i], 0.f);
					Out |= (i ? ECollisionType::Up : ECollisionType::Right);
				}
				else if (Movement[i] < (CollisionReal) 0.0)
				{
					//TryMovement = 0.f;//std::min(Object->getArea().otherCorner()[i] - LastPosition[i], 0.f);
					Out |= (i ? ECollisionType::Down : ECollisionType::Left);
				}
				else
				{
					std::cout << "Null movement, undefined behavior." << std::endl;
				}

				Movement[i] = (CollisionReal) 0.0;
				Area.Position[i] = LastPosition[i];

				if (Area.intersects(Object->getInternalArea())) // If still collision after revert...
				{
					CollisionReal Intersections[2];
					Intersections[0] = Area.getIntersection(Object->getInternalArea()).getArea(); // 0 - old position

					Area.Position[i] = LastPosition[i] + OriginalMovement;
					Intersections[1] = Area.getIntersection(Object->getInternalArea()).getArea(); // 1 - new position

					if (Intersections[1] < Intersections[0] || equals(Intersections[0], Intersections[1]))
					{
						// If new position is better or equal

						//std::cout << "Allowed collision movement (" << (i ? 'y' : 'x') << "): (" << Intersections[0] << " -> " << Intersections[1] << ")" << std::endl;
						AllowedMovement = true;
						Movement[i] = OriginalMovement;

						// Cancel collision output
						if (OriginalMovement > (CollisionReal) 0.0)
						{
							Out ^= (i ? ECollisionType::Up : ECollisionType::Right);
						}
						else if (OriginalMovement < (CollisionReal) 0.f)
						{
							Out ^= (i ? ECollisionType::Down : ECollisionType::Left);
						}
						else
						{
							std::cout << "Null movement, undefined behavior." << std::endl;
						}
					}
					else
					{
						Area.Position[i] = LastPosition[i];
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; ++ i)
		{
			if (Area.intersects(Object->getInternalArea()))
			{
				if (Movement[i] > (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Up : ECollisionType::Right);
				else if (Movement[i] < (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Down : ECollisionType::Left);
			}
		}
	}

	return Out;
}

void CCollisionActor::onStanding(CCollideable * Object)
{
	// Keep reference to object being stood on
	Standing = Object;

	// Cancel gravity acceleration
	FallAcceleration = 0;

	// Stop downwards movement (from impulse, etc.)
	Velocity.Y = std::max(Velocity.Y, (CollisionReal) 0);


	// Adjust to surface below
	static float const MaxStandingAdjustThreshold = 0.1f;

	if (abs(Area.Position.Y - Object->getArea().otherCorner().Y) < MaxStandingAdjustThreshold)
		Area.Position.Y = Object->getArea().otherCorner().Y;
}

bool CCollisionActor::isAbove(CCollisionObject * Object, float & height) const
{
	if (Area.getCenter().Y < Object->getArea().otherCorner().Y)
		return false;

	if (Area.getCenter().X < Object->getArea().Position.X || Area.getCenter().X > Object->getArea().otherCorner().X)
		return false;

	height = (float) Object->getArea().otherCorner().Y;

	return true;
}

bool CCollisionActor::getControlFall()
{
	return ControlFall;
}

void CCollisionActor::updateVectors(CollisionReal const TickTime)
{
	// Manage jumping action
	if (Jumping || Standing) // Can't start jumping unless we are standing
	{
		if (! Jumping && WantsToJump)
			JumpTimer = Attributes.JumpLength; // Start jumping

		Jumping = WantsToJump;
	}

	// zero-gravity hack (FIX)
	if (ControlFall)
		FallAcceleration -= Gravity * TickTime;


	// Walking/Running movement update

	// Determine maximum walking velocity
	CollisionReal MaxVelocity = Attributes.MaxWalk * (Standing ? 1.f : Attributes.AirSpeedFactor);

	bool Moving = false;
	if (Action == EActionType::MoveLeft)
	{
		if (Velocity.X > -MaxVelocity)
		{
			Velocity.X -= Attributes.WalkAccel * TickTime * (Standing ? 1.f : Attributes.AirControl);
			if (Velocity.X < -MaxVelocity)
				Velocity.X = -MaxVelocity;
		}
		Moving = true;
	}
	else if (Action == EActionType::MoveRight)
	{
		if (Velocity.X < MaxVelocity)
		{
			Velocity.X += Attributes.WalkAccel * TickTime * (Standing ? 1.f : Attributes.AirControl);
			if (Velocity.X > MaxVelocity)
				Velocity.X = MaxVelocity;
		}
		Moving = true;
	}
	else if (Action == EActionType::None)
	{
	}

	// Friction when not moving
	if (! Moving)
	{
		if (Standing)
			Velocity.X *= Attributes.GroundStandingFriction; // Slowdown factor
		else
			Velocity.X *= Attributes.AirStandingFriction; // Air Slowdown factor
	}


	// Perform jump
	if (Jumping)
	{
		if (JumpTimer > 0)
		{
			Velocity.Y = Attributes.JumpAccel;
			JumpTimer -= TickTime;
		}

		if (JumpTimer <= 0)
			Jumping = false;
	}

	// Perform impulse
	Velocity += ImpulseVelocity;
	ImpulseVelocity *= 0.7f;

	// Add velocity from gravity
	Velocity.Y += FallAcceleration * TickTime;

	// Keep track of whether a collision-movement was allowed
	AllowedMovement = false;
	// Keep track of whether actor is standing on a surface
	Standing = false;

	// Keep track of last position for movement reversal
	LastPosition = Area.Position;

	// Perform test movement
	Movement = Velocity * TickTime;
	Area.Position += Movement;
}

void CCollisionActor::pushIfCollided(CCollisionObject * Object, SVec2 const Movement)
{
	if (! collidesWith(Object) && Object != Standing)
		return;
	
	Area.Position += Movement;
}

bool const CCollisionActor::isStanding() const
{
	return Standing != 0;
}

void CCollisionActor::setVelocity(SVec2 const & vel)
{
	Velocity = vel;
}

SVec2 const & CCollisionActor::getVelocity() const
{
	return Velocity;
}

CCollisionActor::SAttributes const & CCollisionActor::getAttributes() const
{
	return Attributes;
}

CCollisionActor::SAttributes & CCollisionActor::getAttributes()
{
	return Attributes;
}

void CCollisionActor::setAction(EActionType const & action)
{
	Action = action;
}

bool const CCollisionActor::isJumping() const
{
	return Jumping;
}

CCollisionActor::EActionType const CCollisionActor::getAction() const
{
	return Action;
}

void CCollisionActor::setJumping(bool const jumping)
{
	WantsToJump = jumping;
}

void CCollisionActor::setControlFall(bool const fall)
{
	ControlFall = fall;
}

bool CCollisionActor::updateCollision(CCollideable * Object, float const TickTime)
{
	int CollisionType = checkCollision(Object, TickTime);

	if (CollisionType)
	{
		SCollisionEvent Event;
		Event.This = this;
		Event.Other = Object;
		Event.Direction = (ECollisionType::Domain) CollisionType;

		OnCollision.emit(Event);
	}

	if (CollisionType & ECollisionType::Up)
	{
		Velocity.Y *= -Object->getMaterial().Elasticity;
		Jumping = false;
	}

	if (CollisionType & ECollisionType::Down)
	{
		/*if(Attributes.Bounce > 0.01f)
		{
			Velocity.Y = Attributes.Bounce;
			Attributes.Bounce *= 0.5f;
		}*/
		//Jumping = false;
	}

	//if (Attributes.Reacts && (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right))
	//	Velocity.X *= -Object->getMaterial().Elasticity;

	return (CollisionType & ECollisionType::Down) != 0;
}

void CCollisionActor::draw()
{
	if (! Standing && ! Jumping && ! AllowedMovement)
		glColor3f(1, 1, 1);
	else
		glColor3f(Standing ? 1.f : 0.f, AllowedMovement ? 1.f : 0.f, Jumping ? 1.f : 0.f);

	CCollideable::draw();

	glColor3f(1, 1, 1);
}

void CCollisionActor::addImpulse(SVec2 const & velocity, float const Duration)
{
	Impulses.push_back(std::vector<SVec2, float>(velocity, Duration));
}

void CCollisionActor::setFallAcceleration(CollisionReal speed)
{
	FallAcceleration = speed;
}
