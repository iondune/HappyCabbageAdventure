#include "CShader.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>


CShader::CShader()
{}

GLuint const CShader::getProgramHandle() const
{
    return Handle;
}

std::map<std::string, GLuint> const & CShader::getAttributeHandles() const
{
    return AttributeHandles;
}

std::map<std::string, GLuint> const & CShader::getUniformHandles() const
{
    return UniformHandles;
}
