#ifndef _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_
#define _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_

#include <boost/shared_ptr.hpp>
#include "CShaderContext.h"


struct IUniform
{
	virtual void bind(GLuint const handle, CShaderContext & shaderContext) const = 0;
};

template <typename T>
struct SUniformReference : public IUniform
{
	T const * Value;

	SUniformReference()
		: Value(0)
	{}

	SUniformReference(T const * value)
		: Value(value)
	{}

	SUniformReference(T const & value)
		: Value(& value)
	{}

	void bind(GLuint const handle, CShaderContext & shaderContext) const
	{
		if (Value)
			shaderContext.uniform(handle, * Value);
	}
};

template <typename T>
struct SUniformValue : public IUniform
{
	T Value;

	SUniformValue(T const & value)
		: Value(value)
	{}

	void bind(GLuint const handle, CShaderContext & shaderContext) const
	{
		if (Value)
			shaderContext.uniform(handle, Value);
	}
};


template <typename T>
static boost::shared_ptr<IUniform const> BindUniform(T const & uniform)
{
	return boost::shared_ptr<IUniform const>(new SUniform<T>(uniform));
}

#endif
