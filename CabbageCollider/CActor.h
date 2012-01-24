#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include "../CabbageCore/SRect2.h"

#include "CObject.h"

namespace Cabbage
{
namespace Collider
{
	class CActor
	{

		SRect2 Area;
		SVector2 Acceleration;
		SVector2 Velocity;
		bool Standing;

		SVector2 LastPosition, Movement;

		// Max Velocity <= 0.9f * TickTime * Area.getMaxExtent();

	public:

		CActor();
		~CActor();

		bool collidesWith(CObject * Object);

		bool checkCollision(CObject * Object, float const TickTime);
		void onSurfaceAlight();

		void updateVectors(float const TickTime);

		void pushIfCollided(CObject * Object, SVector2 const & Movement);

		void setArea(SRect2 const & area)
		{
			Area = area;
		}

		SRect2 const & getArea() const
		{
			return Area;
		}

		void setAcceleration(SVector2 const & accel)
		{
			Acceleration = accel;
		}

		SVector2 const & getAcceleration() const
		{
			return Acceleration;
		}

		bool const isStanding() const
		{
			return Standing;
		}

		void setVelocity(SVector2 const & vel)
		{
			Velocity = vel;
		}

		SVector2 const & getVelocity() const
		{
			return Velocity;
		}

	};
}
}

#endif
