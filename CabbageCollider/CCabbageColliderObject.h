#ifndef _CABBAGECOLLIDER_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_COBJECT_H_INCLUDED_

#include <Utils/SRect2.h>

namespace CabbageCollider
{
	class CObject
	{

		SRect2 Area;
		float Friction;

	public:

		CObject(float const friction = 0.9f)
			: Friction(friction)
		{}

		~CObject()
		{}

		SRect2 const & getArea() const
		{
			return Area;
		}

		virtual SVector2 performMovement(float const TickTime)
		{
			return SVector2();
		}

		void setArea(SRect2 const & area)
		{
			Area = area;
		}

		float const getFriction() const
		{
			return Friction;
		}

	};
}


#endif
