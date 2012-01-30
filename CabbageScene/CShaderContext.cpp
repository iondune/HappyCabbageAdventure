#include "CShaderContext.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>


CShaderContext::CShaderContext(CShader const & shader)
	: Shader(shader), Valid(true)
{
	glUseProgram(Shader.Handle);
}

CShaderContext::~CShaderContext()
{
	for (std::vector<GLuint>::const_iterator it = EnabledVertexAttribArrays.begin(); it != EnabledVertexAttribArrays.end(); ++ it)
		glDisableVertexAttribArray(* it);

	glUseProgram(0);
}

void CShaderContext::bindBuffer(std::string const & label, GLuint const BufferHandle, GLuint const ElementSize)
{
	std::map<std::string, GLuint>::const_iterator it = Shader.AttributeHandles.find(label);

	if (it == Shader.AttributeHandles.end())
	{
		std::cerr << "Attribute '" << label << "' was not loaded for shader. Some objects will not draw." << std::endl;
		Valid = false;
		return;
	}

	glEnableVertexAttribArray(it->second);
	glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
	glVertexAttribPointer(it->second, ElementSize, GL_FLOAT, GL_FALSE, 0, 0);

	EnabledVertexAttribArrays.push_back(it->second);
}

void CShaderContext::bindBuffer(GLuint const BufferHandle)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferHandle);
}

void CShaderContext::uniform(std::string const & label, float const uniform)
{
	std::map<std::string, GLuint>::const_iterator it = Shader.UniformHandles.find(label);

	if (it == Shader.UniformHandles.end())
	{
		std::cerr << "Uniform '" << label << "' was not loaded for shader. Some objects will not draw." << std::endl;
		Valid = false;
		return;
	}

	glUniform1f(it->second, uniform);
}

void CShaderContext::uniform(std::string const & label, int const uniform)
{
	std::map<std::string, GLuint>::const_iterator it = Shader.UniformHandles.find(label);

	if (it == Shader.UniformHandles.end())
	{
		std::cerr << "Uniform '" << label << "' was not loaded for shader. Some objects will not draw." << std::endl;
		Valid = false;
		return;
	}

	glUniform1i(it->second, uniform);
}
