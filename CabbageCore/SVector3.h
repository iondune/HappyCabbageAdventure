#ifndef _CABBAGE_CORE_SVECTOR3_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR3_H_INCLUDED_

#include <cmath>

#include "glm/glm.hpp"

class SVector3
{

public:

	float X, Y, Z;

	SVector3()
		: X(0), Y(0), Z(0)
	{}

	SVector3(float in)
		: X(in), Y(in), Z(in)
	{}

	SVector3(float in_x, float in_y, float in_z)
		: X(in_x), Y(in_y), Z(in_z)
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

	SVector3 crossProduct(SVector3 const & v) const
	{
		return SVector3(Y*v.Z - v.Y*Z, v.X*Z - X*v.Z, X*v.Y - v.X*Y);
	}

	float dotProduct(SVector3 const & v) const
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

	SVector3 operator + (SVector3 const & v) const
	{
		return SVector3(X+v.X, Y+v.Y, Z+v.Z);
	}

	SVector3 & operator += (SVector3 const & v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;

		return * this;
	}

	SVector3 operator - (SVector3 const & v) const
	{
		return SVector3(X-v.X, Y-v.Y, Z-v.Z);
	}

	SVector3 & operator -= (SVector3 const & v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;

		return * this;
	}

	SVector3 operator * (SVector3 const & v) const
	{
		return SVector3(X*v.X, Y*v.Y, Z*v.Z);
	}

	SVector3 & operator *= (SVector3 const & v)
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;

		return * this;
	}

	SVector3 operator / (SVector3 const & v) const
	{
		return SVector3(X/v.X, Y/v.Y, Z/v.Z);
	}

	SVector3 & operator /= (SVector3 const & v)
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;

		return * this;
	}

	SVector3 operator * (float const s) const
	{
		return SVector3(X*s, Y*s, Z*s);
	}

	SVector3 & operator *= (float const s)
	{
		X *= s;
		Y *= s;
		Z *= s;

		return * this;
	}

	SVector3 operator / (float const s) const
	{
		return SVector3(X/s, Y/s, Z/s);
	}

	SVector3 & operator /= (float const s)
	{
		X /= s;
		Y /= s;
		Z /= s;

		return * this;
	}

	bool const operator <= (SVector3 const & v) const
	{
		return (X < v.X && Y < v.Y && Z < v.Z);
	}

	bool const operator >= (SVector3 const & v) const
	{
		return (X > v.X && Y > v.Y && Z > v.Z);
	}

};

#endif
