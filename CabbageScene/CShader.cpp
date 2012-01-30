#include "CShader.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>


CShader::CShader()
{}

bool const CShader::loadAttribute(std::string const & label)
{
	GLuint AttributeHandle = glGetAttribLocation(Handle, label.c_str());

	if (AttributeHandle < 0)
	{
		std::cerr << "Failed to find attribute '" << label << "' in shader, some objects may not draw." << std::endl;
		return false;
	}

	AttributeHandles[label] = AttributeHandle;
	return true;
}

bool const CShader::loadUniform(std::string const & label)
{
	GLuint UniformHandle = glGetUniformLocation(Handle, label.c_str());

	if (UniformHandle < 0)
	{
		std::cerr << "Failed to find attribute '" << label << "' in shader, some objects may not draw." << std::endl;
		return false;
	}

	UniformHandles[label] = UniformHandle;
	return true;
}
