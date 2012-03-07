#ifndef _CABBAGE_SCENE_CRENDERBUFFEROBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERBUFFEROBJECT_H_INCLUDED_

#ifdef __unix__
#include<GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif

class CRenderBufferObject
{

	GLuint Handle;

public:

	CRenderBufferObject();
	~CRenderBufferObject();

	GLuint const getHandle();

};

#endif
