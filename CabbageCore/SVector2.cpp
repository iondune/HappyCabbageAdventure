#include "SVector2.h"


SVector2 SVector2Reference::operator + (SVector2Reference const & v) const
{
	return SVector2(X+v.X, Y+v.Y);
}

SVector2 SVector2Reference::operator - (SVector2Reference const & v) const
{
	return SVector2(X-v.X, Y-v.Y);
}

SVector2 SVector2Reference::operator * (SVector2Reference const & v) const
{
	return SVector2(X*v.X, Y*v.Y);
}

SVector2 SVector2Reference::operator / (SVector2Reference const & v) const
{
	return SVector2(X/v.X, Y/v.Y);
}

SVector2 SVector2Reference::operator + (SVector2 const & v) const
{
	return SVector2(X+v.X, Y+v.Y);
}

SVector2 SVector2Reference::operator - (SVector2 const & v) const
{
	return SVector2(X-v.X, Y-v.Y);
}

SVector2 SVector2Reference::operator * (SVector2 const & v) const
{
	return SVector2(X*v.X, Y*v.Y);
}

SVector2 SVector2Reference::operator / (SVector2 const & v) const
{
	return SVector2(X/v.X, Y/v.Y);
}

SVector2 SVector2Reference::operator * (float const s) const
{
	return SVector2(X*s, Y*s);
}

SVector2 SVector2Reference::operator / (float const s) const
{
	return SVector2(X/s, Y/s);
}
