#include "CCamera.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

CCamera::CCamera(float const AspectRatio, float const Near, float const Far, float const FOV)
    : LookDirection(0, 0, 1), Position(0, 0, 0)
{
    recalculateViewMatrix();
    setProjection(FOV, AspectRatio, Near, Far);
}

#include <stdio.h>
void CCamera::recalculateViewMatrix()
{
    //printf("Camera position: <%0.2f, %0.2f, %0.2f>; Camera lookat: <%0.2f, %0.2f, %0.2f>\n", Position.X, Position.Y, Position.Z, LookDirection.X, LookDirection.Y, LookDirection.Z);
    ViewMatrix = glm::lookAt(Position.getGLMVector(), (Position + LookDirection).getGLMVector(), glm::vec3(0, 1, 0));
}

void CCamera::setProjection(float const FOV, float const AspectRatio, float const Near, float const Far)
{
    ProjectionMatrix = glm::perspective(FOV, AspectRatio, Near, Far);
}

SVector3 const & CCamera::getPosition() const
{
    return Position;
}

SVector3 const & CCamera::getLookDirecton() const
{
    return LookDirection;
}

void CCamera::setPosition(SVector3 const & position)
{
    Position = position;
}

void CCamera::setLookDirection(SVector3 const & lookDirection)
{
    LookDirection = lookDirection;
}

glm::mat4 const & CCamera::getViewMatrix() const
{
    return ViewMatrix;
}

glm::mat4 const & CCamera::getProjectionMatrix() const
{
    return ProjectionMatrix;
}
