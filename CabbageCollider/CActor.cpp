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

	CActor::CActor()
		: Standing(false), Jump(false)
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
				if (Movement[i] > 0)
				{
					Movement[i] = Object->getArea().Position[i] - (LastPosition + Area.Size)[i];
					Out |= (i ? ECollisionType::Up : ECollisionType::Right);
				}
				else if (Movement[i] < 0)
				{
					Movement[i] = Object->getArea().otherCorner()[i] - LastPosition[i];
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
		Acceleration.Y = std::max(Acceleration.Y, 0.f);
		Velocity.Y = std::max(Velocity.Y, 0.f);
	}

	bool CActor::collidesWith(CObject * Object)
	{
		return Area.intersects(Object->getArea());
	}

	void CActor::updateVectors(float const TickTime)
	{
		static float const Gravity = 32.f;
		Acceleration.Y -= Gravity * TickTime;
		Acceleration.X = 0;

	
		Velocity.X *=  (Standing ? 0.95f : Attributes.AirCap); 

		if (Action == EActionType::MoveLeft)
		{
			Acceleration.X -= Attributes.MoveAccel * (Standing ? 1 : Attributes.AirMod);
		}

		if (Action == EActionType::MoveRight)
		{
			Acceleration.X += Attributes.MoveAccel * (Standing ? 1 : Attributes.AirMod);
		}

		if (Jump && Standing)
		{
			
			Velocity.Y = Attributes.JumpSpeed;
		}

		Jump = false;

		Velocity += Acceleration * TickTime;

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
		glPushMatrix();
		glTranslatef(Area.Position.X, Area.Position.Y, 0);

		if (Standing)
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 1);

		glBegin(GL_QUADS);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(Area.Size.X, 0.f, 0.f);
			glVertex3f(Area.Size.X, Area.Size.Y, 0.f);
			glVertex3f(0.f, Area.Size.Y, 0.f);
		glEnd();

		glColor3f(1, 1, 1);
		glPopMatrix();
	}

}
}
