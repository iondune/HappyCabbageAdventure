#ifndef _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_
#define _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_

#include "CShaderContext.h"

struct IUniform
{
	virtual void bind(GLint const handle, CShaderContext & shaderContext) =0;
};

template <typename T>
struct SUniform : public IUniform
{
	T * Value;

	SUniform()
		: Value(0)
	{}

	SUniform(T * value)
		: Value(value)
	{}

	SUniform(T & value)
		: Value(& value)
	{}

	void bind(GLint const handle, CShaderContext & shaderContext)
	{
		if (Value)
			shaderContext.uniform(handle, * Value);
	}
};

#endif
