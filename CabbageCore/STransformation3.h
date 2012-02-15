#ifndef _CABBAGE_CORE_STRANSFORMATION3_H_INCLUDED_
#define _CABBAGE_CORE_STRANSFORMATION3_H_INCLUDED_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SVector3.h"

class STransformation3
{

public:

	glm::mat4 Translation, Rotation, Scale;

	STransformation3()
		: Translation(1.f), Rotation(1.f), Scale(1.f)
	{}

	glm::mat4 const getTransformation()
	{
		return Translation * Rotation * Scale;
	}

	glm::mat4 const get() const
	{
		return Translation * Rotation * Scale;
	}

	glm::mat4 const operator() () const
	{
		return Translation * Rotation * Scale;
	}

	void setRotation(glm::mat4 const & rotation)
	{
		Rotation = rotation;
	}

	void setRotation(glm::vec3 const & rotation)
	{
		Rotation = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1, 0, 0));
		Rotation = glm::rotate(Rotation, rotation.y, glm::vec3(0, 1, 0));
		Rotation = glm::rotate(Rotation, rotation.z, glm::vec3(0, 0, 1));
	}

	void setRotation(SVector3 const & rotation)
	{
		Rotation = glm::rotate(glm::mat4(1.f), rotation.X, glm::vec3(1, 0, 0));
		Rotation = glm::rotate(Rotation, rotation.Y, glm::vec3(0, 1, 0));
		Rotation = glm::rotate(Rotation, rotation.Z, glm::vec3(0, 0, 1));
	}

	void setScale(glm::vec3 const & scale)
	{
		Scale = glm::scale(glm::mat4(1.f), scale);
	}

	void setScale(SVector3 const & scale)
	{
		Scale = glm::scale(glm::mat4(1.f), scale.getGLMVector());
	}

	void setTranslation(glm::vec3 const & translation)
	{
		Translation = glm::translate(glm::mat4(1.f), translation);
	}

	void setTranslation(SVector3 const & translation)
	{
		Translation = glm::translate(glm::mat4(1.f), translation.getGLMVector());
	}

};

#endif
