#ifndef _CABBAGE_SCENE_CCAMERASCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CCAMERASCENEOBJECT_H_INCLUDED_

#include "ICameraSceneObject.h"


class CCameraSceneObject : public ICameraSceneObject
{

protected:

	SVector3f Position;
	SVector3f LookDirection;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

public:

	CCameraSceneObject();

	virtual void recalculateViewMatrix();

	virtual SVector3f const & getLookDirecton() const;
	virtual SVector3f const getLookAtTarget() const;

	virtual void setLookDirection(SVector3f const & lookDirection);
	virtual void setLookAtTarget(SVector3f const & lookAtTarget);

	virtual glm::mat4 const & getViewMatrix() const;
	virtual glm::mat4 const & getProjectionMatrix() const;

};

#endif
