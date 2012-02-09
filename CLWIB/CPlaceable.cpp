#include "CPlaceable.h"

std::string CPlaceable::getX()
{
    std::stringstream xValue;
    xValue << x;
    return xValue.str();
}
std::string CPlaceable::getY()
{
    std::stringstream yValue;
    yValue << y;
    return yValue.str();
}
std::string CPlaceable::getWidth()
{
    std::stringstream widthValue;
    widthValue << w;
    return widthValue.str();
}
std::string CPlaceable::getHeight()
{
    std::stringstream heightValue;
    heightValue << h;
    return heightValue.str();
}