#ifndef _CABBAGE_CORE_SRECT2_H_INCLUDED_
#define _CABBAGE_CORE_SRECT2_H_INCLUDED_

#include "SVector2.h"
#include "Utils.h"

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

	bool const intersects(SRect2 const & r) const
	{
		return (otherCorner().Y > r.Position.Y || equals(otherCorner().Y, r.Position.Y) ) && 
			(Position.Y < r.otherCorner().Y || equals(Position.Y, r.otherCorner().Y) ) && 
			(otherCorner().X > r.Position.X || equals(otherCorner().X, r.Position.X) ) && 
			(Position.X < r.otherCorner().X || equals(Position.X, r.otherCorner().X) );
	}

	SRect2 const getIntersection(SRect2 const & r) const
	{
		SVector2 Position(std::max(r.Position.X, Position.X), std::max(r.Position.Y, Position.Y));
		SVector2 OtherCorner(std::min(r.otherCorner().X, otherCorner().X), std::min(r.otherCorner().Y, otherCorner().Y));

		return SRect2(Position, OtherCorner - Position);
	}

	bool const isPointInside(SVector2 const & v) const
	{
		return (otherCorner().Y > v.Y && 
			Position.Y < v.Y && 
			otherCorner().X > v.X && 
			Position.X < v.X);
	}

	float const getArea() const
	{
		return Size.X * Size.Y;
	}

};

#endif
