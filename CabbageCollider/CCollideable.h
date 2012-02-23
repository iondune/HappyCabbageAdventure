#ifndef _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_
#define _CABBAGECOLLIDER_CCOLLIDEABLE_H_INCLUDED_

#include "../CabbageCore/SRect2.h"
#define COLLIDEABLE_TYPE_BLOCK 10
#define COLLIDEABLE_TYPE_ELEVATOR 11
#define COLLIDEABLE_TYPE_ACTOR 12
#define COLLIDEABLE_TYPE_APPLE 13
#define COLLIDEABLE_TYPE_ORANGE 14
#define COLLIDEABLE_TYPE_KIWI 15
#define COLLIDEABLE_TYPE_FLAME 16

#define COLLIDEABLE_TYPE_PLAYER 500
#define COLLIDEABLE_TYPE_COLLIDEABLE -1 

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

      int CollideableType;

		virtual void draw();

	};
}
}

#endif
