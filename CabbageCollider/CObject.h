#ifndef _CABBAGECOLLIDER_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_COBJECT_H_INCLUDED_

#include "CCollideable.h"

namespace Cabbage
{
namespace Collider
{
	class CObject : public CCollideable
	{

	protected:

		friend class CEngine;

		CObject()
		{}

	public:

		~CObject()
		{}


		virtual SVector2 performMovement(float const TickTime)
		{
			return SVector2();
		}

		void draw();

	};
}
}

#endif
