#ifndef _CABBAGE_CORE_SVECTOR3_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR3_H_INCLUDED_

#include <cmath>

#include "glm/glm.hpp"

#include "Utils.h"
#include "SVector2.h"

class SVector3;

class SVector3Reference
{

public:

    float & X, & Y, & Z;

    SVector3Reference(float & x, float & y, float & z)
        : X(x), Y(y), Z(z)
	{}

    glm::vec3 const getGLMVector() const
    {
        return glm::vec3(X, Y, Z);
    }

    float const operator[] (unsigned int i) const
    {
        switch (i)
        {
        default:
        case 0:
            return X;
        case 1:
            return Y;
        case 2:
            return Z;
        }
    }

    float & operator[] (unsigned int i)
    {
        switch (i)
        {
        default:
        case 0:
            return X;
        case 1:
            return Y;
        case 2:
            return Z;
        }
    }

    SVector3 crossProduct(SVector3Reference const & v) const;

    SVector3 const getNormalized() const;

    SVector3 operator + (SVector3 const & v) const;

    SVector3 operator - (SVector3 const & v) const;

    SVector3 operator * (SVector3 const & v) const;

    SVector3 operator / (SVector3 const & v) const;

    SVector3 operator * (float const s) const;

    SVector3 operator / (float const s) const;

    float dotProduct(SVector3Reference const & v) const
    {
        return X*v.X + Y*v.Y + Z*v.Z;
    }

    void normalize()
    {
        float const len = length();

        X /= len;
        Y /= len;
        Z /= len;
    }

    float length() const
    {
        return sqrtf(X*X + Y*Y + Z*Z);
    }

    SVector3Reference & operator += (SVector3Reference const & v)
    {
        X += v.X;
        Y += v.Y;
        Z += v.Z;

        return * this;
    }

    SVector3Reference & operator -= (SVector3Reference const & v)
    {
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;

        return * this;
    }

    SVector3Reference & operator *= (SVector3Reference const & v)
    {
        X *= v.X;
        Y *= v.Y;
        Z *= v.Z;

        return * this;
    }

    SVector3Reference & operator /= (SVector3Reference const & v)
    {
        X /= v.X;
        Y /= v.Y;
        Z /= v.Z;

        return * this;
    }

    SVector3Reference & operator *= (float const s)
    {
        X *= s;
        Y *= s;
        Z *= s;

        return * this;
    }

    SVector3Reference & operator /= (float const s)
    {
        X /= s;
        Y /= s;
        Z /= s;

        return * this;
    }

    bool const operator <= (SVector3Reference const & v) const
    {
        return (X < v.X && Y < v.Y && Z < v.Z);
    }

    bool const operator >= (SVector3Reference const & v) const
    {
        return (X > v.X && Y > v.Y && Z > v.Z);
    }

    bool const operator == (SVector3Reference const & v) const
    {
        return equals(v);
    }

	bool const operator != (SVector3Reference const & v) const
    {
        return ! equals(v);
    }

	bool const equals(SVector3Reference const & v, float const Epsilon = RoundingError32) const
    {
        return (::equals(X, v.X, Epsilon) && ::equals(Y, v.Y, Epsilon) && ::equals(Z, v.Z, Epsilon));
    }

	SVector2Reference const xy() const
    {
        return SVector2Reference(X, Y);
    }

    SVector2Reference const xz() const
    {
        return SVector2Reference(X, Z);
    }

    SVector2Reference const yz() const
    {
        return SVector2Reference(Y, Z);
    }

};

class SVector3 : public SVector3Reference
{

public:

    float X, Y, Z;

    SVector3()
        : X(0), Y(0), Z(0), SVector3Reference(X, Y, Z)
    {}

	SVector3(SVector3Reference const & vec)
        : X(vec.X), Y(vec.Y), Z(vec.Z), SVector3Reference(X, Y, Z)
    {}

	SVector3(SVector3 const & vec)
        : X(vec.X), Y(vec.Y), Z(vec.Z), SVector3Reference(X, Y, Z)
    {}

    SVector3(SVector2 xy, float z)
		: X(xy.X), Y(xy.Y), Z(z), SVector3Reference(X, Y, Z)
    {}

    SVector3(glm::vec3 const & vec)
        : X(vec.x), Y(vec.y), Z(vec.z), SVector3Reference(X, Y, Z)
    {}

    SVector3(float in)
        : X(in), Y(in), Z(in), SVector3Reference(X, Y, Z)
    {}

    SVector3(float in_x, float in_y, float in_z)
        : X(in_x), Y(in_y), Z(in_z), SVector3Reference(X, Y, Z)
    {}

	SVector3 & operator = (SVector3Reference const & vec)
	{
		X = vec.X;
		Y = vec.Y;
		Z = vec.Z;

		return * this;
	}

	SVector3 & operator = (SVector3 const & vec)
	{
		X = vec.X;
		Y = vec.Y;
		Z = vec.Z;

		return * this;
	}

};
typedef SVector3 SVector3f;

#endif
