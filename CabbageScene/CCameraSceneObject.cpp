#include "CCameraSceneObject.h"

#include <glm/gtc/matrix_transform.hpp>


CCameraSceneObject::CCameraSceneObject()
    : LookDirection(0, 0, 1)
{
    recalculateViewMatrix();
}

void CCameraSceneObject::recalculateViewMatrix()
{
    //printf("Camera position: <%0.2f, %0.2f, %0.2f>; Camera lookat: <%0.2f, %0.2f, %0.2f>\n", Position.X, Position.Y, Position.Z, LookDirection.X, LookDirection.Y, LookDirection.Z);
    ViewMatrix = glm::lookAt(getPosition().getGLMVector(), (getPosition() + LookDirection).getGLMVector(), glm::vec3(0, 1, 0));
}

SVector3f const & CCameraSceneObject::getLookDirecton() const
{
    return LookDirection;
}

SVector3f const CCameraSceneObject::getLookAtTarget() const
{
	return getPosition() + LookDirection;
}

void CCameraSceneObject::setLookDirection(SVector3f const & lookDirection)
{
    LookDirection = lookDirection;
}

void CCameraSceneObject::setLookAtTarget(SVector3f const & lookAtTarget)
{
	setLookDirection(lookAtTarget - getPosition());
}

glm::mat4 const & CCameraSceneObject::getViewMatrix() const
{
    return ViewMatrix;
}

glm::mat4 const & CCameraSceneObject::getProjectionMatrix() const
{
    return ProjectionMatrix;
}
