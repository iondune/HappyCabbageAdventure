#ifndef _CABBAGE_CORE_SVECTOR2_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR2_H_INCLUDED_

#include <cmath>
#include <stdio.h>

template <typename T>
class SVector2;

template <typename T>
class SVector2Reference
{

public:

	T & X, & Y;

	SVector2Reference(T & x, T & y)
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

	void set(T in)
	{
		X = in;
		Y = in;
	}

	void set(T in_x, T in_y)
	{
		X = in_x;
		Y = in_y;
	}

	T dotProduct(SVector2Reference<T> const & v) const
	{
		return X*v.X + Y*v.Y;
	}

	T const length() const
	{
		return sqrtf(X*X + Y*Y);
	}

	SVector2<T> operator + (SVector2Reference<T> const & v) const;

	SVector2<T> operator - (SVector2Reference<T> const & v) const;

	SVector2<T> operator * (SVector2Reference<T> const & v) const;

	SVector2<T> operator / (SVector2Reference<T> const & v) const;

	SVector2<T> operator + (SVector2<T> const & v) const;

	SVector2<T> operator - (SVector2<T> const & v) const;

	SVector2<T> operator * (SVector2<T> const & v) const;

	SVector2<T> operator / (SVector2<T> const & v) const;

	SVector2<T> operator * (T const s) const;

	SVector2<T> operator / (T const s) const;

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

};

template <typename T>
class SVector2f : public SVector2Reference<T>
{

public:

	T X, Y;

	SVector2()
		: X(0), Y(0), SVector2Reference(X, Y)
	{}

	template <typename U>
	SVector2(SVector2<U> const & vec)
		: X(vec.X), Y(vec.Y), SVector2Reference(X, Y)
	{}
	
	template <typename U>
	SVector2(SVector2Reference<U> const & vec)
		: X(vec.X), Y(vec.Y), SVector2Reference(X, Y)
	{}

	SVector2(T in)
		: X(in), Y(in), SVector2Reference(X, Y)
	{}

	SVector2(T in_x, T in_y)
		: X(in_x), Y(in_y), SVector2Reference(X, Y)
	{}

	SVector2<T> & operator = (SVector2Reference<T> const & vec)
	{
		X = vec.X;
		Y = vec.Y;

		return * this;
	}

	SVector2<T> & operator = (SVector2<T> const & vec)
	{
		X = vec.X;
		Y = vec.Y;

		return * this;
	}

};

template <typename T>
SVector2<T> SVector2Reference<T>::operator + (SVector2Reference<T> const & v) const
{
	return SVector2<T>(X+v.X, Y+v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator - (SVector2Reference<T> const & v) const
{
	return SVector2<T>(X-v.X, Y-v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator * (SVector2Reference<T> const & v) const
{
	return SVector2<T>(X*v.X, Y*v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator / (SVector2Reference<T> const & v) const
{
	return SVector2<T>(X/v.X, Y/v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator + (SVector2<T> const & v) const
{
	return SVector2<T>(X+v.X, Y+v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator - (SVector2<T> const & v) const
{
	return SVector2<T>(X-v.X, Y-v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator * (SVector2<T> const & v) const
{
	return SVector2<T>(X*v.X, Y*v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator / (SVector2<T> const & v) const
{
	return SVector2<T>(X/v.X, Y/v.Y);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator * (T const s) const
{
	return SVector2<T>(X*s, Y*s);
}

template <typename T>
SVector2<T> SVector2Reference<T>::operator / (T const s) const
{
	return SVector2<T>(X/s, Y/s);
}

typedef SVector2<float> SVector2f;
typedef SVector2<double> SVector2d;

#endif
