#ifndef _CABBAGE_SCENE_CSHADERCONTEXT_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADERCONTEXT_H_INCLUDED_

#include "CShader.h"

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
    void bindBufferObject(std::string const & label, GLuint const BufferHandle, GLuint const ElementSize);

    /*!
     * Binds an element array buffer
     */
    void bindIndexBufferObject(GLuint const BufferHandle);

    /*!
     * Send uniform variables to shader
     */
    void uniform(std::string const & label, float const uniform);
    void uniform(std::string const & label, int const uniform);
    void uniform(std::string const & labal, glm::mat4 const & uniform);

};


#endif
