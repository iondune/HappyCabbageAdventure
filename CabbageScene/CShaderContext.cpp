#include "CShaderContext.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>

#include "../CabbageCore/glm/gtc/type_ptr.hpp"


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

void CShaderContext::bindBufferObject(std::string const & label, GLuint const BufferHandle, GLuint const ElementSize)
{
    std::map<std::string, SShaderVariable>::const_iterator it = Shader.AttributeHandles.find(label);

    if (it == Shader.AttributeHandles.end())
    {
        std::cerr << "Attribute '" << label << "' was not loaded for shader. Some objects will not draw." << std::endl;
        Valid = false;
        return;
    }

    glEnableVertexAttribArray(it->second.Handle);
    glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
    glVertexAttribPointer(it->second.Handle, ElementSize, GL_FLOAT, GL_FALSE, 0, 0);

    EnabledVertexAttribArrays.push_back(it->second.Handle);
}

void CShaderContext::bindBufferObject(GLuint const attribHandle, GLuint const bufferHandle, GLuint const elementSize)
{
    glEnableVertexAttribArray(attribHandle);
    glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
    glVertexAttribPointer(attribHandle, elementSize, GL_FLOAT, GL_FALSE, 0, 0);

    EnabledVertexAttribArrays.push_back(attribHandle);
}

void CShaderContext::bindIndexBufferObject(GLuint const BufferHandle)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferHandle);
}

void CShaderContext::uniform(GLuint const uniformHandle, float const uniform)
{
    glUniform1f(uniformHandle, uniform);
}

void CShaderContext::uniform(GLuint const uniformHandle, int const uniform)
{
    glUniform1i(uniformHandle, uniform);
}

void CShaderContext::uniform(GLuint const uniformHandle, glm::mat4 const & uniform)
{
    glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, glm::value_ptr(uniform));
}

void CShaderContext::uniform(GLuint const uniformHandle, STransformation3 const & uniform)
{
	glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, glm::value_ptr(uniform()));
}

void CShaderContext::uniform(GLuint const uniformHandle, SVector3 const & uniform)
{
    glUniform3f(uniformHandle, uniform.X, uniform.Y, uniform.Z);
}

void CShaderContext::uniform(GLuint const uniformHandle, SColor const & uniform)
{
    glUniform3f(uniformHandle, uniform.Red, uniform.Green, uniform.Blue);
}
