#ifndef _CABBAGE_SCENE_CSHADER_H_INCLUDED_
#define _CABBAGE_SCENE_CSHADER_H_INCLUDED_

#include <string>
#include <map>
#include <vector>

#ifdef __unix__
#include<GL/gl.h>
#include<GL/glu.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#endif


class CShader
{

	friend class CShaderLoader;
	friend class CShaderContext;

	GLuint Handle;
	std::map<std::string, GLuint> AttributeHandles;
	std::map<std::string, GLuint> UniformHandles;

	CShader();

public:

    GLuint const getProgramHandle() const;

};


#endif
