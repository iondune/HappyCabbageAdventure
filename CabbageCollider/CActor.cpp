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

	CActor::SAttributes::SAttributes()
		: MoveAccel(50.8f), JumpSpeed(8.f), AirMod(0.3f), AirCap(0.97f)
	{}


	CActor::EActionType::EActionType()
		: Value(Standing)
	{}

	CActor::EActionType::EActionType(Domain const value)
		: Value(value)
	{}

	bool const CActor::EActionType::operator == (Domain const value)
	{
		return Value == value;
	}


	CActor::CActor()
		: Standing(false), Jump(false), FallAcceleration(0)
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
		static float const Gravity = 32.f;
		FallAcceleration -= Gravity * TickTime;
		//Acceleration.X = 0;

	
		Velocity.X *=  (Standing ? 0.95f : Attributes.AirCap); 

		if (Action == EActionType::MoveLeft)
		{
			//Acceleration.X -= Attributes.MoveAccel * (Standing ? 1 : Attributes.AirMod);
		}

		if (Action == EActionType::MoveRight)
		{
			//Acceleration.X += Attributes.MoveAccel * (Standing ? 1 : Attributes.AirMod);
		}

		if (Jump && Standing)
		{
			
			Velocity.Y = Attributes.JumpSpeed;
		}

		Jump = false;

		//Velocity += Acceleration * TickTime;

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

	void CActor::setAcceleration(SVector2 const & accel)
	{
		Acceleration = accel;
	}

	SVector2 const & CActor::getAcceleration() const
	{
		return Acceleration;
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

	void CActor::jump()
	{
		Jump = true;
	}


	bool CActor::updateCollision(CCollideable * Object, float const TickTime)
	{
		int CollisionType = checkCollision(Object, TickTime);

		if (CollisionType & ECollisionType::Up)
			Velocity.Y *= Object->getMaterial().Elasticity;

		if (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right)
			Velocity.X *= Object->getMaterial().Elasticity;

		return (CollisionType & ECollisionType::Down) != 0;
	}

	void CActor::draw()
	{
		if (Standing)
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 1);

		CCollideable::draw();

		glColor3f(1, 1, 1);
	}

}
}
