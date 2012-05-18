#ifndef _CABBAGE_SCENE_CCAMERA_H_INCLUDED_
#define _CABBAGE_SCENE_CCAMERA_H_INCLUDED_

#include <SVector3.h>
#include <glm/glm.hpp>

class ICamera
{

protected:

    SVector3f Position;
    SVector3f LookDirection;

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

public:

	ICamera();

	virtual void recalculateViewMatrix();

	SVector3f const & getPosition() const;
    SVector3f const & getLookDirecton() const;

	void setPosition(SVector3f const & position);
    void setLookDirection(SVector3f const & lookDirection);

	glm::mat4 const & getViewMatrix() const;
    glm::mat4 const & getProjectionMatrix() const;

};

class CPerspectiveCamera : public ICamera
{

public:

    CPerspectiveCamera(float const AspectRatio = 16.f/9.f, float const Near = 0.1f, float const Far = 100.f, float const FOV = 60.f);

    virtual void setProjection(float const FOV, float const AspectRatio, float const Near, float const Far);

};

#endif
