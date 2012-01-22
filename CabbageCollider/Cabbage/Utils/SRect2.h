#ifndef _CABBAGEUTILS_SRECT2_H_
#define _CABBAGEUTILS_SRECT2_H_

#include "SVector2.h"

class SRect2
{

public:

	SVector2 Position, Size;

	SRect2()
	{}

	SRect2(SVector2 const & position, SVector2 const & size)
		: Position(position), Size(size)
	{}

	SRect2(float const x, float const y, float const w, float const h)
		: Position(x, y), Size(w, h)
	{}

	SVector2 const otherCorner() const
	{
		return Position + Size;
	}

	SVector2 const getCenter() const
	{
		return Position + Size / 2.f;
	}

	bool const intersects(SRect2 const & r)
	{
		return (otherCorner().Y > r.Position.Y &&
			Position.Y < r.otherCorner().Y &&
			otherCorner().X > r.Position.X &&
			Position.X < r.otherCorner().X);
	}

};

#endif
