#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/boost/shared_ptr.hpp"
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

    CMat4Uniform();
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

struct SMaterial
{
    CShader * Shader;
    CTexture * Texture;

    SMaterial();
};

class CScene;

class CRenderable
{

protected:

    // Model Transformation
    SVector3 Translation, Rotation, Scale;

    // Implicit uniforms
    boost::shared_ptr<CMat4Uniform> uModelMatrix, uNormalMatrix;
    SBoundingBox3 BoundingBox;

public:

    struct SAttribute
    {
        GLint Handle;
        boost::shared_ptr<IAttribute> Value;

        SAttribute();
        SAttribute(boost::shared_ptr<IAttribute> value);
    };

    struct SUniform
    {
        GLint Handle;
        boost::shared_ptr<IUniform> Value;

        SUniform();
        SUniform(boost::shared_ptr<IUniform> value);
    };

protected:

    std::map<std::string, SAttribute> Attributes;
    std::map<std::string, SUniform> Uniforms;
    std::map<std::string, SUniform> SceneLoadedUniforms;

    CBufferObject<GLushort> * IndexBufferObject;

    SMaterial Material;

    CShader * NormalColorShader;
    CRenderable * NormalObject;

    GLenum DrawType;
    int DebugDataFlags;

    void loadHandlesFromShader(CShader const * const shader, CScene const * const scene);
    CScene const * LastLoadedScene;
    CShader const * LastLoadedShader;

public:

    CRenderable();

    SVector3 const & getTranslation() const;
    SVector3 const & getRotation() const;
    SVector3 const & getScale() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setScale(SVector3 const & scale);

    SMaterial & getMaterial();
    SMaterial const & getMaterial() const;

    CBufferObject<GLushort> * getIndexBufferObject();
    void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

    GLenum const getDrawType() const;
    void setDrawType(GLenum const drawType);

    void draw(CScene const * const scene);

    SBoundingBox3 const & getBoundingBox() const;
    SBoundingBox3 & getBoundingBox();
    void setBoundingBox(SBoundingBox3 const & boundingBox);

    void addAttribute(std::string const & label, boost::shared_ptr<IAttribute> attribute);
    void addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform);
    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

    bool const isDebugDataEnabled(EDebugData::Domain const type) const;
    void enableDebugData(EDebugData::Domain const type);
    void disableDebugData(EDebugData::Domain const type);

    bool const intersectsWithLine(SLine3 const & line) const;

};

#endif
