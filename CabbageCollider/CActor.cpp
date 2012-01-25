#include "CActor.h"

#include <algorithm>

#include "../CabbageCore/Utils.h"

#ifdef __unix__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif

namespace Cabbage
{
namespace Collider
{

	static float const Gravity = 55.f;

	CActor::SAttributes::SAttributes()
		: MaxWalk(4.5f), WalkAccel(60.f), JumpAccel(6.4f), JumpLength(0.6f), AirControl(0.75f), AirSpeedFactor(0.7f)
	{}


	CActor::EActionType::EActionType()
		: Value(None)
	{}

	CActor::EActionType::EActionType(Domain const value)
		: Value(value)
	{}

	bool const CActor::EActionType::operator == (Domain const value)
	{
		return Value == value;
	}


	CActor::CActor()
		: Standing(false), JumpTimer(0.f), FallAcceleration(0)
	{}

	CActor::~CActor()
	{}

	int CActor::checkCollision(CCollideable * Object, float const TickTime)
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

	void CActor::onStanding()
	{
		Standing = true;
		FallAcceleration = 0;
		Velocity.Y = std::max(Velocity.Y, 0.f);
	}

	bool CActor::collidesWith(CObject * Object) const
	{
		return Area.intersects(Object->getArea());
	}

	bool CActor::isAbove(CObject * Object, float & height) const
	{
		if (Area.getCenter().Y < Object->getArea().otherCorner().Y)
			return false;

		if (Area.getCenter().X < Object->getArea().Position.X || Area.getCenter().X > Object->getArea().otherCorner().X)
			return false;

		height = Object->getArea().otherCorner().Y;

		return true;
	}

	void CActor::updateVectors(float const TickTime)
	{
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
				Velocity.X *= 0.95f; // Slowdown factor
			else
				Velocity.X *= 0.99f; // Air Slowdown factor
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

		Velocity.Y += FallAcceleration * TickTime;

		Standing = false;

		LastPosition = Area.Position;
		Movement = Velocity * TickTime;
	}

	void CActor::pushIfCollided(CObject * Object, SVector2 const & Movement)
	{
		if (! collidesWith(Object))
			return;

		for (int i = 0; i < 2; ++ i)
		{
			if (Movement[i] > 0)
			{
				Area.Position[i] += Object->getArea().otherCorner()[i] - Area.Position[i];
			}
			else if (Movement[i] < 0)
			{
				Area.Position[i] -= Area.otherCorner()[i] - Object->getArea().Position[i];
			}
		}
	}

	bool const CActor::isStanding() const
	{
		return Standing;
	}

	void CActor::setVelocity(SVector2 const & vel)
	{
		Velocity = vel;
	}

	SVector2 const & CActor::getVelocity() const
	{
		return Velocity;
	}

	CActor::SAttributes const & CActor::getAttributes() const
	{
		return Attributes;
	}

	void CActor::setAction(EActionType const & action)
	{
		Action = action;
	}

	void CActor::setJumping(bool const jumping)
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

	bool CActor::updateCollision(CCollideable * Object, float const TickTime)
	{
		int CollisionType = checkCollision(Object, TickTime);

		if (CollisionType & ECollisionType::Up)
		{
			Velocity.Y *= -Object->getMaterial().Elasticity;
			Jumping = false;
		}

		if (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right)
			Velocity.X *= -Object->getMaterial().Elasticity;

		return (CollisionType & ECollisionType::Down) != 0;
	}

	void CActor::draw()
	{
		if (! Standing && ! Jumping)
			glColor3f(1, 1, 1);
		else
			glColor3f(Standing ? 1.f : 0.f, 0.f, Jumping ? 1.f : 0.f);

		CCollideable::draw();

		glColor3f(1, 1, 1);
	}

}
}
