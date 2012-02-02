#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
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
        if (BufferObject->isDirty())
            BufferObject->syncData();

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

class CFloatUniform : public IUniform
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

class CIntUniform : public IUniform
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

class CMat4Uniform : public IUniform
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


class CRenderable
{

protected:

    SVector3 Translation, Rotation, Scale;
    SBoundingBox3 BoundingBox;

    struct SAttribute
    {
        GLint Handle;
        IAttribute * Value;

        SAttribute()
            : Value(0), Handle(-1)
        {}

        SAttribute(IAttribute * value)
            : Value(value), Handle(-1)
        {}
    };

    struct SUniform
    {
        GLint Handle;
        IUniform * Value;

        SUniform()
            : Value(0), Handle(-1)
        {}

        SUniform(IUniform * value)
            : Value(value), Handle(-1)
        {}
    };

    std::map<std::string, SAttribute> Attributes;
    std::map<std::string, SUniform> Uniforms;
    std::set<GLenum const> RenderModes;

    CBufferObject<GLushort> * IndexBufferObject;
    CShader * Shader;
    CTexture * Texture;

    CRenderable * NormalObject;

    GLenum DrawType;

public:

    CRenderable();

    SVector3 const & getTranslation() const;
    SVector3 const & getRotation() const;
    SVector3 const & getScale() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setScale(SVector3 const & scale);

    CShader * getShader();
    void setShader(CShader * shader);

    CTexture * getTexture();
    void setTexture(CTexture * texture);

    CBufferObject<GLushort> * getIndexBufferObject();
    void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

    GLenum const getDrawType() const;
    void setDrawType(GLenum const drawType);

    void draw(CCamera const & Camera);

    SBoundingBox3 const & getBoundingBox() const;
    SBoundingBox3 & getBoundingBox();
    void setBoundingBox(SBoundingBox3 const & boundingBox);

    void addAttribute(std::string const & label, IAttribute * attribute);
    void addUniform(std::string const & label, IUniform * uniform);
    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

    void addRenderMode(GLenum const mode);
    void removeRenderMode(GLenum const mode);

};

#endif
