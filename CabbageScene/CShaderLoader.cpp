#include "CShaderLoader.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>


static inline bool printOpenGLErrors()
{
    bool Succeeded = true;

    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << gluErrorString(glErr) << std::endl;
        Succeeded = false;
        glErr = glGetError();
    }

    return Succeeded;
}

static inline void printShaderInfoLog(GLuint shaderHandle)
{
    printOpenGLErrors();

    GLint infoLogLength = 0;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, & infoLogLength);

    if (infoLogLength > 0)
    {
        GLchar * infoLog = new GLchar[infoLogLength];
        GLint charsWritten = 0;
        glGetShaderInfoLog(shaderHandle, infoLogLength, & charsWritten, infoLog);
        std::cout << "Shader Info Log:" << std::endl << infoLog << std::endl;
        delete infoLog;
    }
}

static inline void printProgramInfoLog (GLuint programHandle)
{
    printOpenGLErrors();

    GLint infoLogLength = 0;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, & infoLogLength);

    if (infoLogLength > 0)
    {
        GLchar * infoLog = new GLchar[infoLogLength];
        GLint charsWritten = 0;
        glGetProgramInfoLog(programHandle, infoLogLength, & charsWritten, infoLog);
        std::cout << "Program Info Log:" << std::endl << infoLog << std::endl;
        delete infoLog;
    }
}

std::map<std::string, CShader *> CShaderLoader::LoadedShaders;

std::string CShaderLoader::ShaderDirectory = "../Media/Shaders/";

CShader * const CShaderLoader::loadShader(std::string const & name)
{
    std::map<std::string, CShader *>::iterator it = LoadedShaders.find(name);

    if (it != LoadedShaders.end())
    {
        return it->second;
    }

    std::string const vertFileName = ShaderDirectory + name + ".vert";
    std::string const fragFileName = ShaderDirectory + name + ".frag";

    // Create OpenGL Shader objects
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

    // Read Vertex Shader file
    {
        std::ifstream vertFile(vertFileName.c_str());
        if (! vertFile.is_open())
        {
            std::cerr << "Could not open file of shader: " << vertFileName << std::endl;
            glDeleteShader(VS);
            glDeleteShader(FS);
            return 0;
        }
        std::string vertFileSource;

        vertFile.seekg(0, std::ios::end);   
        vertFileSource.reserve((size_t) vertFile.tellg());
        vertFile.seekg(0, std::ios::beg);

        vertFileSource.assign((std::istreambuf_iterator<char>(vertFile)), std::istreambuf_iterator<char>());
        GLchar const * SourceString = vertFileSource.c_str();
        glShaderSource(VS, 1, & SourceString, NULL);
    }


    // Read Fragment Shader file
    {
        std::ifstream fragFile(fragFileName.c_str());
        if (! fragFile.is_open())
        {
            std::cerr << "Could not open file of shader: " << fragFileName << std::endl;
            glDeleteShader(VS);
            glDeleteShader(FS);
            return 0;
        }
        std::string fragFileSource;

        fragFile.seekg(0, std::ios::end);   
        fragFileSource.reserve((size_t) fragFile.tellg());
        fragFile.seekg(0, std::ios::beg);

        fragFileSource.assign((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
        GLchar const * SourceString = fragFileSource.c_str();
        glShaderSource(FS, 1, & SourceString, NULL);
    }


    // Compile Shader
    {
        glCompileShader(VS);
        GLint vCompiled;
        glGetShaderiv(VS, GL_COMPILE_STATUS, & vCompiled);
        printShaderInfoLog(VS);
        if (! vCompiled)
        {
            std::cerr << "Error compiling shader: " << vertFileName << std::endl;
            glDeleteShader(VS);
            glDeleteShader(FS);
            return 0;
        }
    }

    {
        glCompileShader(FS);
        GLint fCompiled;
        glGetShaderiv(FS, GL_COMPILE_STATUS, & fCompiled);
        printShaderInfoLog(FS);
        if (! fCompiled)
        {
            std::cerr << "Error compiling shader: " << fragFileName << std::endl;
            glDeleteShader(VS);
            glDeleteShader(FS);
            return 0;
        }
    }

    CShader * Shader = new CShader();

    // Create Shader program
    Shader->Handle = glCreateProgram();
    glAttachShader(Shader->Handle, VS);
    glAttachShader(Shader->Handle, FS);

    // Link program
    {
        GLint linked;
        glLinkProgram(Shader->Handle);
        glGetProgramiv(Shader->Handle, GL_LINK_STATUS, & linked);
        printProgramInfoLog(Shader->Handle);
    }

    std::cout << "Sucessfully installed shader! (" << Shader->Handle << ")" << std::endl;

    // Load all attributes and save handles
    GLint total = 0;
    GLint longestName = 0;
    glGetProgramiv(Shader->Handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, & longestName);
    glGetProgramiv(Shader->Handle, GL_ACTIVE_ATTRIBUTES, & total);
    for (GLint i = 0; i < total; ++ i)
    {
        GLsizei nameLenth = -1, variableSize = -1;
        GLenum dataType = GL_ZERO;
        char * nameBuffer = new char[longestName + 1];
        glGetActiveAttrib(Shader->Handle, i, longestName, & nameLenth, & variableSize, & dataType, nameBuffer);
        nameBuffer[nameLenth] = 0;
        GLuint variableLocation = glGetAttribLocation(Shader->Handle, nameBuffer);
        Shader->AttributeHandles[nameBuffer] = SShaderVariable(variableLocation, dataType);
        delete nameBuffer;
    }

    // Load all uniforms and save handles
    total = 0;
    longestName = 0;
    glGetProgramiv(Shader->Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, & longestName);
    glGetProgramiv(Shader->Handle, GL_ACTIVE_UNIFORMS, & total);
    for (GLint i = 0; i < total; ++ i)
    {
        GLsizei nameLenth = -1, variableSize = -1;
        GLenum dataType = GL_ZERO;
        char * nameBuffer = new char[longestName + 1];
        glGetActiveUniform(Shader->Handle, i, longestName, & nameLenth, & variableSize, & dataType, nameBuffer);
        nameBuffer[nameLenth] = 0;
        GLuint variableLocation = glGetUniformLocation(Shader->Handle, nameBuffer);
        Shader->UniformHandles[nameBuffer] = SShaderVariable(variableLocation, dataType);
        delete nameBuffer;
    }

    LoadedShaders[name] = Shader;
    return Shader;
}
