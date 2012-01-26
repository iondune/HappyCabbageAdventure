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

			SMaterial();
		};

	protected:

		friend class CEngine;

		SMaterial Material;

		SRect2 Area;

		CCollideable();

	public:

		virtual ~CCollideable();

		SRect2 const & getArea() const;
		void setArea(SRect2 const & area);

		SMaterial const & getMaterial() const;
		SMaterial & getMaterial();

		virtual void draw();

	};
}
}

#endif
