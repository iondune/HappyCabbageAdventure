#ifndef _CABBAGE_CORE_SBOUNDINGBOX2_H_INCLUDED_
#define _CABBAGE_CORE_SBOUNDINGBOX2_H_INCLUDED_

#include "SVector3.h"

class SBoundingBox3
{

public:

    SVector3 MinCorner, MaxCorner;

    SBoundingBox3()
    {}

    SBoundingBox3(SVector3 const & min, SVector3 const & max)
        : MinCorner(min), MaxCorner(max)
    {}

    SVector3 const getExtent() const
    {
        return MaxCorner - MinCorner;
    }

    SVector3 const getCenter() const
    {
        return (MaxCorner + MinCorner) / 2.f;
    }

    bool const intersects(SBoundingBox3 const & r) const
    {
        return (MaxCorner.Y > r.MinCorner.Y &&
            MinCorner.Y < r.MaxCorner.Y &&
            MaxCorner.X > r.MinCorner.X &&
            MinCorner.X < r.MaxCorner.X &&
            MaxCorner.Z > r.MinCorner.Z &&
            MinCorner.Z < r.MaxCorner.Z);
    }

    void addInternalPoint(SVector3 const & v)
    {
        if (v.X > MaxCorner.X)
            MaxCorner.X = v.X;
        if (v.Y > MaxCorner.Y)
            MaxCorner.Y = v.Y;
        if (v.Z > MaxCorner.Z)
            MaxCorner.Z = v.Z;

        if (v.X < MinCorner.X)
            MinCorner.X = v.X;
        if (v.Y < MinCorner.Y)
            MinCorner.Y = v.Y;
        if (v.Z < MinCorner.Z)
            MinCorner.Z = v.Z;
    }

};

#endif
