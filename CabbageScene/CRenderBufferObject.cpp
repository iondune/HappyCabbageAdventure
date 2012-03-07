#include "CRenderBufferObject.h"

CRenderBufferObject::CRenderBufferObject()
{
	glGenRenderbuffers(1, & Handle);
	glBindRenderbuffer(GL_RENDERBUFFER, Handle);
}

CRenderBufferObject::~CRenderBufferObject()
{
	glDeleteRenderbuffers(1, & Handle);
}

GLuint const CRenderBufferObject::getHandle()
{
	return Handle;
}
