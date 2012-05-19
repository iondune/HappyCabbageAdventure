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
	: CCollideable(), Standing(0), JumpTimer(0.f), FallAcceleration(0), Impulse(false), ControlFall(true) 
{
	CollideableType = COLLIDEABLE_TYPE_ACTOR;
	CollideableLevel = INTERACTOR_ACTORS;
	CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_ACTORS;

	Gravity = 100.0f;
}

CCollisionActor::~CCollisionActor()
{}

int CCollisionActor::checkCollision(CCollideable * Object, float const TickTime)
{
	int Out = ECollisionType::None;

	float const BufferSize = 0.0001f;

	Area.Position = LastPosition;

	for (int i = 0; i < 2; ++ i)
	{
		Area.Position[i] = LastPosition[i] + Movement[i];

		if (Area.intersects(Object->getArea()))
		{
			if (Movement[i] > 0.f)
			{
				Movement[i] = std::max(Object->getArea().Position[i] - (LastPosition + Area.Size)[i], 0.f);
				Out |= (i ? ECollisionType::Up : ECollisionType::Right);
			}
			else if (Movement[i] < 0.f)
			{
				Movement[i] = std::min(Object->getArea().otherCorner()[i] - LastPosition[i], 0.f);
				Out |= (i ? ECollisionType::Down : ECollisionType::Left);
			}
		}

		Area.Position[i] = LastPosition[i] + Movement[i];
	}

	return Out;
}

int CCollisionActor::ignoreCollision(CCollideable * Object, float const TickTime)
{
	int Out = ECollisionType::None;

	float const BufferSize = 0.0001f;

	Area.Position = LastPosition;

	for (int i = 0; i < 2; ++ i)
	{
		Area.Position[i] = LastPosition[i] + Movement[i];

		if (Area.intersects(Object->getArea()))
		{
			if (Movement[i] > 0.f)
			{
				Out |= (i ? ECollisionType::Up : ECollisionType::Right);
			}
			else if (Movement[i] < 0.f)
			{
				Out |= (i ? ECollisionType::Down : ECollisionType::Left);
			}
		}

		Area.Position[i] = LastPosition[i] + Movement[i];
	}

	return Out;
}

void CCollisionActor::setStanding(bool s) {
	Standing = s?this:0;
}

void CCollisionActor::onStanding(CCollideable * Object)
{
	Standing = Object;
	FallAcceleration = 0;
	Velocity.Y = std::max(Velocity.Y, 0.f);
}

bool CCollisionActor::collidesWith(CCollisionObject * Object) const
{
	return Area.intersects(Object->getArea());
}

bool CCollisionActor::isAbove(CCollisionObject * Object, float & height) const
{
	if (Area.getCenter().Y < Object->getArea().otherCorner().Y)
		return false;

	if (Area.getCenter().X < Object->getArea().Position.X || Area.getCenter().X > Object->getArea().otherCorner().X)
		return false;

	height = Object->getArea().otherCorner().Y;

	return true;
}

bool CCollisionActor::getControlFall() {
	return ControlFall;
}

void CCollisionActor::setFallAcceleration(float fa) {
   FallAcceleration = fa;
}

void CCollisionActor::updateVectors(float const TickTime)
{
	if (ControlFall)
		FallAcceleration -= Gravity * TickTime;

	float MaxVelocity = Attributes.MaxWalk * (Standing ? 1.f : Attributes.AirSpeedFactor);
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

	if (! Moving)
	{
		if (Standing)
			Velocity.X *= Attributes.GroundStandingFriction; // Slowdown factor
		else
			Velocity.X *= Attributes.AirStandingFriction; // Air Slowdown factor
	}


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

	if (Impulse)
	{
		if (ImpulseTimer > 0)
		{
			Velocity = ImpulseVelocity;
			ImpulseTimer -= TickTime;
		}

		if (ImpulseTimer <= 0)
			Impulse = false;
	}

	Velocity.Y += FallAcceleration * TickTime;

	Standing = false;

	LastPosition = Area.Position;
	Movement = Velocity * TickTime;
}

void CCollisionActor::pushIfCollided(CCollisionObject * Object, SVector2 const & Movement)
{
	if (! collidesWith(Object) && Object != Standing)
		return;

	Area.Position += Movement;

	/*for (int i = 0; i < 2; ++ i)
	{
	if (Movement[i] > 0)
	{
	Area.Position[i] += Object->getArea().otherCorner()[i] - Area.Position[i];
	}
	else if (Movement[i] < 0)
	{
	Area.Position[i] -= Area.otherCorner()[i] - Object->getArea().Position[i];
	}
	}*/
}

bool const CCollisionActor::isStanding() const
{
	return Standing != 0;
}

void CCollisionActor::setVelocity(SVector2 const & vel)
{
	Velocity = vel;
}

SVector2 const & CCollisionActor::getVelocity() const
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

bool CCollisionActor::isJumping()
{
	return Jumping;
}

CCollisionActor::EActionType CCollisionActor::getAction() {
	return Action;
}

void CCollisionActor::setJumping(bool const jumping)
{
	if (! Jumping && ! Standing)
		return; // Can't start jupming unless we are standing

	if (! Jumping && jumping)
		JumpTimer = Attributes.JumpLength; // Start jumping

	Jumping = jumping;
	/*if (Standing && ! Jumping)
	{
	JumpTimer = Attributes.JumpLength;
	Jumping = true;
	}*/
}

void CCollisionActor::setControlFall(bool const fall) {
	ControlFall = fall;
}

bool CCollisionActor::updateCollision(CCollideable * Object, float const TickTime)
{
	int CollisionType = checkCollision(Object, TickTime);

	if (CollisionType) {
      if(CollisionResponder)
         CollisionResponder->OnCollision(Object);
      if(Object->CollisionResponder)
         Object->CollisionResponder->OnCollision(this);
   }
   if (CollisionType & ECollisionType::Up)
   {
      Velocity.Y *= -Object->getMaterial().Elasticity;
      Jumping = false;
   }

   if (CollisionType & ECollisionType::Down)
   {
      if(Attributes.Bounce > 0.01f) {
         Velocity.Y = Attributes.Bounce;
         Attributes.Bounce *= 0.5f;
      }
      //Jumping = false;
   }

   if (Attributes.Reacts && (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right))
      Velocity.X *= -Object->getMaterial().Elasticity;
   return (CollisionType & ECollisionType::Down) != 0;
}

void CCollisionActor::draw()
{
	if (! Standing && ! Jumping)
		glColor3f(1, 1, 1);
	else
		glColor3f(Standing ? 1.f : 0.f, 0.f, Jumping ? 1.f : 0.f);

	CCollideable::draw();

	glColor3f(1, 1, 1);
}

void CCollisionActor::setImpulse(SVector2 const & velocity, float const duration)
{
	Impulse = true;
	ImpulseTimer = duration;
	ImpulseVelocity = velocity;
}

void CCollisionActor::addImpulse(SVector2 const & velocity) {
	Impulse = true;
	ImpulseVelocity += velocity;
}


void CCollisionActor::setGravity(float grav)
{
	Gravity = grav;
}

float CCollisionActor::getGravity()
{
   return Gravity;
}
