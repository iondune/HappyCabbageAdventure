#include "CPerspectiveCameraSceneObject.h"


CPerspectiveCamera::CPerspectiveCamera(float const AspectRatio, float const Near, float const Far, float const FOV)
{
    setProjection(FOV, AspectRatio, Near, Far);
}

void CPerspectiveCamera::setProjection(float const FOV, float const AspectRatio, float const Near, float const Far)
{
    ProjectionMatrix = glm::perspective(FOV, AspectRatio, Near, Far);
}
