#ifndef _CABBAGE_CORE_SCOLOR_H_INCLUDED_
#define _CABBAGE_CORE_SCOLOR_H_INCLUDED_

class SColor
{

public:

    float Red, Green, Blue;

    SColor()
        : Red(0.7f), Green(0.7f), Blue(0.7f)
    {}

	SColor(float red, float green, float blue)
        : Red(red), Green(green), Blue(blue)
    {}

	float const operator[] (unsigned int i) const
	{
		switch (i)
		{
		default:
		case 0:
			return Red;
		case 1:
			return Green;
		case 2:
			return Blue;
		}
	}

	float & operator[] (unsigned int i)
	{
		switch (i)
		{
		default:
		case 0:
			return Red;
		case 1:
			return Green;
		case 2:
			return Blue;
		}
	}

};

#endif
