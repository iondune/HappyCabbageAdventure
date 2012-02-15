#ifndef _CABBAGE_SCENE_CSHADERCONTEXT_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADERCONTEXT_H_INCLUDED_

#include "CShader.h"

#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SColor.h"
#include "../CabbageCore/Stransformation3.h"
#include "../CabbageCore/glm/glm.hpp"

class CShaderContext
{

    bool Valid;
    CShader const & Shader;

    std::vector<GLuint> EnabledVertexAttribArrays;

public:

    /*!
     * Creates a shader context, used for drawing an object with the shader
     */
    CShaderContext(CShader const & shader);

    /*!
     * Cleans up shader call... make sure that the CShaderContext leaves scope before another context is created!
     */
    ~CShaderContext();

    /*!
     * Binds a shader attribute variable to a VBO.
     */
    void bindBufferObject(std::string const & label, GLuint const bufferHandle, GLuint const elementSize);
    void bindBufferObject(GLuint const attribHandle, GLuint const bufferHandle, GLuint const elementSize);

    /*!
     * Binds an element array buffer
     */
    void bindIndexBufferObject(GLuint const BufferHandle);

    /*!
     * Send uniform variables to shader
     */
    void uniform(GLuint const uniformHandle, float const uniform);
    void uniform(GLuint const uniformHandle, int const uniform);
    void uniform(GLuint const uniformHandle, glm::mat4 const & uniform);
    void uniform(GLuint const uniformHandle, STransformation3 const & uniform);
    void uniform(GLuint const uniformHandle, SVector3 const & uniform);
    void uniform(GLuint const uniformHandle, SColor const & uniform);

	template <typename T>
    void CShaderContext::uniform(std::string const & label, T const & uniform)
	{
		std::map<std::string, SShaderVariable>::const_iterator it = Shader.UniformHandles.find(label);

		if (it == Shader.UniformHandles.end())
		{
			std::cerr << "Uniform '" << label << "' was not loaded for shader. Some objects will not draw." << std::endl;
			Valid = false;
			return;
		}

		uniform(it->second.Handle, uniform);
	}


};


#endif
