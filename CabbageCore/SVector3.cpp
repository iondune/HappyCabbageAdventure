#include "SVector3.h"

SVector3 SVector3Reference::crossProduct(SVector3Reference const & v) const
{
    return SVector3(Y*v.Z - v.Y*Z, v.X*Z - X*v.Z, X*v.Y - v.X*Y);
}

SVector3 const SVector3Reference::getNormalized() const
{
    SVector3 copy(* this);
    copy.normalize();
    return copy;
}

SVector3 SVector3Reference::operator + (SVector3 const & v) const
{
    return SVector3(X+v.X, Y+v.Y, Z+v.Z);
}

SVector3 SVector3Reference::operator - (SVector3 const & v) const
{
    return SVector3(X-v.X, Y-v.Y, Z-v.Z);
}

SVector3 SVector3Reference::operator * (SVector3 const & v) const
{
    return SVector3(X*v.X, Y*v.Y, Z*v.Z);
}

SVector3 SVector3Reference::operator / (SVector3 const & v) const
{
    return SVector3(X/v.X, Y/v.Y, Z/v.Z);
}

SVector3 SVector3Reference::operator * (float const s) const
{
    return SVector3(X*s, Y*s, Z*s);
}

SVector3 SVector3Reference::operator / (float const s) const
{
    return SVector3(X/s, Y/s, Z/s);
}
