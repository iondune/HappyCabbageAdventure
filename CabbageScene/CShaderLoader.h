#ifndef _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_

#include "CShader.h"

class CShaderLoader
{

    bool Valid;

public:

    /*!
     * Load a vertex shader.
     */
    static CShader * const loadShader(std::string const & vertFileName);

    /*!
     * Load a vertex shader and fragment shader.
     */
    static CShader * const loadShader(std::string const & vertFileName, std::string const & fragFileName);

};


#endif
