#ifndef _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_
#define _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_

#include "CShaderContext.h"
#include "CBufferObject.h"

struct IAttribute
{
	virtual void bind(CShaderContext & shaderContext) =0;
};

template <typename T>
struct SAttribute : public IAttribute
{
	GLint Handle;
	CBufferObject<T> * Buffer;
	int ElementSize;

	SAttribute()
		: Value(0), ElementSize(0)
	{}

	SAttribute(T * value, int const elementSize = 4)
		: Value(value), ElementSize(elementSize)
	{}

	void bind(CShaderContext & shaderContext)
	{
		if (Buffer)
			shaderContext.bindBufferObject(Handle, Buffer->getHandle(), ElementSize);
	}
};

#endif
