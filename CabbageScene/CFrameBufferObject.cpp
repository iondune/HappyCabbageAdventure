#include "CFrameBufferObject.h"

CFrameBufferObject::CFrameBufferObject()
{
	glGenFramebuffers(1, & Handle);
}

CFrameBufferObject::~CFrameBufferObject()
{
	glDeleteFramebuffers(1, & Handle);
}

void CFrameBufferObject::attach(CRenderBufferObject * RenderBufferObject, GLenum const Attachment)
{
	glBindRenderbuffer(GL_RENDERBUFFER, Handle);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, Attachment, GL_RENDERBUFFER, RenderBufferObject->getHandle());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CFrameBufferObject::attach(CTexture * Texture, GLenum const Attachment)
{
	glBindRenderbuffer(GL_RENDERBUFFER, Handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, Texture->getTextureHandle(), 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint const CFrameBufferObject::getHandle()
{
	return Handle;
}

void CFrameBufferObject::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, Handle);
}
