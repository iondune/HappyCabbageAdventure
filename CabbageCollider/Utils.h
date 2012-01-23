/*
#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

bool const equals(float const a, float const b, float const epsilon = 0.00001f)
{
	return fabsf(a) <= epsilon;
}

#endif
*/
#ifndef _CABBAGE_CORE_UTILS_H_INCLUDED_
#define _CABBAGE_CORE_UTILS_H_INCLUDED_

#include <cmath>

const float RoundingError32 = 0.00001f;

bool const equals(float const a, float const b, float const epsilon = RoundingError32)
{
	return (a + epsilon >= b) && (a - epsilon <= b);
}

#endif
