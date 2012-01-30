#ifndef _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_

#include "CShader.h"

class CShaderLoader
{

    bool Valid;

public:

    /*!
     * Initializes the shader loader by checking that the graphic hardware can load shaders.
     */
    CShaderLoader();

    /*!
     * Check to see if our system can load shaders.
     */
    bool const isValid();

    /*!
     * Load a vertex shader.
     */
    CShader * const loadShader(std::string const & vertFileName);

    /*!
     * Load a vertex shader and fragment shader.
     */
    CShader * const loadShader(std::string const & vertFileName, std::string const & fragFileName);

};


#endif
