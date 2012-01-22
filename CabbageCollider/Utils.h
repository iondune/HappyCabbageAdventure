#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

bool const equals(float const a, float const b, float const epsilon = 0.00001f)
{
	return fabsf(a) <= epsilon;
}

#endif
