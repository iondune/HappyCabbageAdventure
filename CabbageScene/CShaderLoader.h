#ifndef _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADERLOADER_H_INCLUDED_

#include "CShader.h"

class CShaderLoader
{

    static std::map<std::string, CShader *> LoadedShaders;

public:

    static CShader * const loadShader(std::string const & name);

    static std::string ShaderDirectory;

};


#endif
