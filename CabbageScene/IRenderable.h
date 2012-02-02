#ifndef _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_IRENDERABLE_H_INCLUDED_

#include <map>

#include "../CabbageCore/SVector3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShaderContext.h"

class IAttribute
{

public:

    virtual void bindTo(GLuint const attribHandle, CShaderContext & shaderContext) =0;

};

class CFloatVecAttribute : public IAttribute
{

public:

    CFloatVecAttribute(CBufferObject<float> * bufferObject, int const size)
        : BufferObject(bufferObject), Size(size)
    {}

    void bindTo(GLuint const attribHandle, CShaderContext & shaderContext)
    {
        shaderContext.bindBufferObject(attribHandle, BufferObject->getHandle(), Size);
    }

    CBufferObject<float> * BufferObject;
    int Size;

};

class IUniform
{

public:

    virtual void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext) =0;

};

class CFloatUniform
{

public:

    CFloatUniform(float const value)
        : Value(value)
    {}

    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
    {
        shaderContext.uniform(uniformHandle, Value);
    }

    float Value;

};

class CIntUniform
{

public:

    CIntUniform(int const value)
        : Value(value)
    {}

    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
    {
        shaderContext.uniform(uniformHandle, Value);
    }

    int Value;

};

class CMat4Uniform
{

public:

    CMat4Uniform(glm::mat4 const & value)
        : Value(value)
    {}

    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
    {
        shaderContext.uniform(uniformHandle, Value);
    }

    glm::mat4 Value;

};


class IRenderable
{

protected:

    SVector3 Translation, Rotation, Scale;

    IRenderable();

    std::map<std::string, IAttribute *> Attributes;
    std::map<std::string, IUniform *> Uniforms;

public:

    SVector3 const & getTranslation() const;
    SVector3 const & getRotation() const;
    SVector3 const & getScale() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setScale(SVector3 const & scale);

    virtual void draw(CCamera const & Camera) =0;

};

#endif
