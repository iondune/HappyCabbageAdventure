#ifndef _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_
#define _CABBAGE_SCENE_SUNIFORM_H_INCLUDED_

#include <boost/shared_ptr.hpp>
#include "CShaderContext.h"


struct IUniform
{
	virtual void bind(GLuint const handle, CShaderContext & shaderContext) const =0;
};

template <typename T>
struct SUniform : public IUniform
{
	T const * Value;

	SUniform()
		: Value(0)
	{}

	SUniform(T const * value)
		: Value(value)
	{}

	SUniform(T const & value)
		: Value(& value)
	{}

	void bind(GLuint const handle, CShaderContext & shaderContext) const
	{
		if (Value)
			shaderContext.uniform(handle, * Value);
	}
};

template <typename T>
static boost::shared_ptr<IUniform const> BindUniform(T const & uniform)
{
	return boost::shared_ptr<IUniform const>(new SUniform<T>(uniform));
}

#endif
