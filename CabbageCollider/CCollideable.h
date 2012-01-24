#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include "../CabbageCore/SRect2.h"

namespace Cabbage
{
namespace Collider
{
	class CCollideable
	{

	public:

		struct SMaterial
		{
			float Friction;
			float Elasticity;

			SMaterial()
				: Friction(1.f), Elasticity(0.f)
			{}
		};

	protected:

		friend class CEngine;

		SMaterial Material;

		SRect2 Area;

		CCollideable()
		{}

	public:

		~CCollideable()
		{}

		SRect2 const & getArea() const
		{
			return Area;
		}

		void setArea(SRect2 const & area)
		{
			Area = area;
		}

		SMaterial const & getMaterial() const
		{
			return Material;
		}

	};
}
}

#endif
