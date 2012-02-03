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

    CFloatVecAttribute(CBufferObject<float> * bufferObject, int const size);
    void bindTo(GLuint const attribHandle, CShaderContext & shaderContext);

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

    CFloatUniform(float const value);
    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext);

    float Value;

};

class CIntUniform : public IUniform
{

public:

    CIntUniform(int const value);
    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext);

    int Value;

};

class CMat4Uniform : public IUniform
{

public:

    CMat4Uniform(glm::mat4 const & value);
    void bindTo(GLuint const uniformHandle, CShaderContext & shaderContext);

    glm::mat4 Value;

};


class EDebugData
{

public:

    enum Domain
    {
        None = 0,
        Normals = 1,
        NormalColors = 2
    };

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

        SAttribute();
        SAttribute(IAttribute * value);
    };

    struct SUniform
    {
        GLint Handle;
        IUniform * Value;

        SUniform();
        SUniform(IUniform * value);
    };

    std::map<std::string, SAttribute> Attributes;
    std::map<std::string, SUniform> Uniforms;
    std::set<GLenum> RenderModes;

    CBufferObject<GLushort> * IndexBufferObject;
    CShader * Shader;
    CShader * NormalColorShader;
    CTexture * Texture;

    CRenderable * NormalObject;

    GLenum DrawType;
    int DebugDataFlags;

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

    bool const isDebugDataEnabled(EDebugData::Domain const type) const;
    void enableDebugData(EDebugData::Domain const type);
    void disableDebugData(EDebugData::Domain const type);

};

#endif
