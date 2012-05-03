#include "SVector2.h"


float SVector2Reference::getDistanceFrom(SVector2Reference const & v) const
{
	return (v - * this).length();
}

SVector2 SVector2Reference::getInterpolated(SVector2Reference const & v, float const d)
{
	float inv = 1.0f - d;
	return SVector2((v.X*inv + X*d), (v.Y*inv + Y*d));
}

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
