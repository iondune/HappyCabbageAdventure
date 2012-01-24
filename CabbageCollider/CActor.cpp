#include "CActor.h"

#include <algorithm>

#include "../CabbageCore/Utils.h"
#include <stdio.h>

namespace Cabbage
{
namespace Collider
{

	CActor::CActor()
		: Standing(false)
	{}

	CActor::~CActor()
	{}

	bool CActor::checkCollision(CObject * Object, float const TickTime)
	{
		bool VerticalCollision = false;

		float const FrictionConstant = Object->getFriction();
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
					if (Movement[i] < 0)
						printf("shit");
				}
				else if (Movement[i] < 0)
				{
					Movement[i] = Object->getArea().otherCorner()[i] - LastPosition[i];
					if (Movement[i] > 0)
						printf("shit");

					if (i == 1)
					{
						VerticalCollision = true;
					}
				}
			}

			Area.Position[i] = LastPosition[i] + Movement[i];
		}

		return VerticalCollision;
	}

	void CActor::onSurfaceAlight()
	{
		Standing = true;
		Acceleration.Y = std::max(Acceleration.Y, 0.f);
		Velocity.Y = std::max(Velocity.Y, 0.f);
		if (equals(Acceleration.X, 0))
			Velocity.X *= 0.95f; // Slowdown Constant
	}
	bool CActor::collidesWith(CObject * Object)
	{
		return Area.intersects(Object->getArea());
	}

	void CActor::updateVectors(float const TickTime)
	{
		static float const Gravity = 22.f;
		Acceleration.Y -= Gravity * TickTime;

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

}
}
