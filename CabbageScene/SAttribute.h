#ifndef _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_
#define _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_

#include "CShaderContext.h"
#include "CBufferObject.h"

struct IAttribute
{
	virtual void bind(GLint const handle, CShaderContext & shaderContext) =0;
};

template <typename T>
struct SAttribute : public IAttribute
{
	CBufferObject<T> * Buffer;
	int ElementSize;

	SAttribute()
		: Value(0), ElementSize(0)
	{}

	SAttribute(T * value, int const elementSize = 4)
		: Value(value), ElementSize(elementSize)
	{}

	void bind(GLint const handle, CShaderContext & shaderContext)
	{
		if (Buffer)
			shaderContext.bindBufferObject(handle, Buffer->getHandle(), ElementSize);
	}
};

#endif
