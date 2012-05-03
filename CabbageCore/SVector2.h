#ifndef _CABBAGE_CORE_SVECTOR2_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR2_H_INCLUDED_

#include <cmath>
#include <stdio.h>

#include "Utils.h"

class SVector2;

class SVector2Reference
{

public:

	float & X, & Y;

	SVector2Reference(float & x, float & y)
		: X(x), Y(y)
	{}

	float const operator[] (int i) const
	{
		return (i == 0 ? X : Y);
	}

	float & operator[] (int i)
	{
		return (i == 0 ? X : Y);
	}

	void reset()
	{
		X = 0;
		Y = 0;
	}

	void set(float in)
	{
		X = in;
		Y = in;
	}

	void set(float in_x, float in_y)
	{
		X = in_x;
		Y = in_y;
	}

	float dotProduct(SVector2Reference const & v) const
	{
		return X*v.X + Y*v.Y;
	}

	float length() const
	{
		return sqrtf(X*X + Y*Y);
	}

	SVector2 operator + (SVector2Reference const & v) const;

	SVector2 operator - (SVector2Reference const & v) const;

	SVector2 operator * (SVector2Reference const & v) const;

	SVector2 operator / (SVector2Reference const & v) const;

	SVector2 operator + (SVector2 const & v) const;

	SVector2 operator - (SVector2 const & v) const;

	SVector2 operator * (SVector2 const & v) const;

	SVector2 operator / (SVector2 const & v) const;

	SVector2 operator * (float const s) const;

	SVector2 operator / (float const s) const;

	SVector2Reference & operator += (SVector2Reference const & v)
	{
		X += v.X;
		Y += v.Y;

		return * this;
	}

	SVector2Reference & operator -= (SVector2Reference const & v)
	{
		X -= v.X;
		Y -= v.Y;

		return * this;
	}

	SVector2Reference & operator *= (SVector2Reference const & v)
	{
		X *= v.X;
		Y *= v.Y;

		return * this;
	}

	SVector2Reference & operator /= (SVector2Reference const & v)
	{
		X /= v.X;
		Y /= v.Y;

		return * this;
	}

	SVector2Reference & operator *= (float const s)
	{
		X *= s;
		Y *= s;

		return * this;
	}

	SVector2Reference & operator /= (float const s)
	{
		X /= s;
		Y /= s;

		return * this;
	}

	void normalize()
    {
        float const len = length();

        X /= len;
        Y /= len;
    }

	bool const operator == (SVector2Reference const & v) const
    {
        return equals(v);
    }

	bool const operator != (SVector2Reference const & v) const
    {
        return ! equals(v);
    }

	bool const equals(SVector2Reference const & v, float const Epsilon = RoundingError32) const
    {
        return ::equals(X, v.X, Epsilon) && ::equals(Y, v.Y, Epsilon);
    }

};

class SVector2 : public SVector2Reference
{

public:

	float X, Y;

	SVector2()
		: X(0), Y(0), SVector2Reference(X, Y)
	{}

	SVector2(SVector2 const & vec)
		: X(vec.X), Y(vec.Y), SVector2Reference(X, Y)
	{}

	SVector2(SVector2Reference const & vec)
		: X(vec.X), Y(vec.Y), SVector2Reference(X, Y)
	{}

	SVector2(float in)
		: X(in), Y(in), SVector2Reference(X, Y)
	{}

	SVector2(float in_x, float in_y)
		: X(in_x), Y(in_y), SVector2Reference(X, Y)
	{}

	SVector2 & operator = (SVector2Reference const & vec)
	{
		X = vec.X;
		Y = vec.Y;

		return * this;
	}

	SVector2 & operator = (SVector2 const & vec)
	{
		X = vec.X;
		Y = vec.Y;

		return * this;
	}

};
typedef SVector2 SVector2f;

#endif
