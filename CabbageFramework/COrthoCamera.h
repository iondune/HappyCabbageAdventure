#ifndef _CABBAGE_SCENE_CCAMERA_H_INCLUDED_
#define _CABBAGE_SCENE_CCAMERA_H_INCLUDED_

#include <SVector3.h>
#include <glm/glm.hpp>

class COrthoCamera : public ICamera
{

public:

	COrthoCamera(float const Near, float const Far);

    COrthoCamera(float const Left, float const Right, float const Bottom, float const Top, float const Near, float const Far);

    virtual void setProjection(float const Left, float const Right, float const Bottom, float const Top, float const Near, float const Far);

};

#endif
