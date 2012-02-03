#include "CRenderable.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"


CFloatVecAttribute::CFloatVecAttribute(CBufferObject<float> * bufferObject, int const size)
    : BufferObject(bufferObject), Size(size)
{}

void CFloatVecAttribute::bindTo(GLuint const attribHandle, CShaderContext & shaderContext)
{
    if (BufferObject->isDirty())
        BufferObject->syncData();

    shaderContext.bindBufferObject(attribHandle, BufferObject->getHandle(), Size);
}

CFloatUniform::CFloatUniform(float const value)
    : Value(value)
{}

void CFloatUniform::bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
{
    shaderContext.uniform(uniformHandle, Value);
}

CIntUniform::CIntUniform(int const value)
    : Value(value)
{}

void CIntUniform::bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
{
    shaderContext.uniform(uniformHandle, Value);
}

CMat4Uniform::CMat4Uniform(glm::mat4 const & value)
    : Value(value)
{}

void CMat4Uniform::bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
{
    shaderContext.uniform(uniformHandle, Value);
}


CRenderable::SAttribute::SAttribute()
    : Value(0), Handle(-1)
{}

CRenderable::SAttribute::SAttribute(IAttribute * value)
    : Value(value), Handle(-1)
{}


CRenderable::SUniform::SUniform()
    : Value(0), Handle(-1)
{}

CRenderable::SUniform::SUniform(IUniform * value)
    : Value(value), Handle(-1)
{}


CRenderable::CRenderable()
    : Scale(1), Shader(0), Texture(0), DrawType(GL_TRIANGLES), NormalObject(0), DebugDataFlags(0), NormalColorShader(0)
{}

SVector3 const & CRenderable::getTranslation() const
{
    return Translation;
}

SVector3 const & CRenderable::getRotation() const
{
    return Rotation;
}

SVector3 const & CRenderable::getScale() const
{
    return Scale;
}


void CRenderable::setTranslation(SVector3 const & translation)
{
    Translation = translation;
}

void CRenderable::setRotation(SVector3 const & rotation)
{
    Rotation = rotation;
}

void CRenderable::setScale(SVector3 const & scale)
{
    Scale = scale;
}

CShader * CRenderable::getShader()
{
    return Shader;
}

void CRenderable::setShader(CShader * shader)
{
    for (std::map<std::string, SAttribute>::iterator it = Attributes.begin(); it != Attributes.end(); ++ it)
        it->second.Handle = -1;

    for (std::map<std::string, SUniform>::iterator it = Uniforms.begin(); it != Uniforms.end(); ++ it)
        it->second.Handle = -1;

    Shader = shader;

    if (! Shader)
        return;

    for (std::map<std::string, SShaderVariable>::const_iterator it = Shader->getAttributeHandles().begin(); it != Shader->getAttributeHandles().end(); ++ it)
    {
        std::map<std::string, SAttribute>::iterator jt = Attributes.find(it->first);
        if (jt != Attributes.end())
            jt->second.Handle = it->second.Handle;
        else
            std::cout << "Attribute required by shader but not found in renderable: " << it->first << std::endl;
    }

    for (std::map<std::string, SShaderVariable>::const_iterator it = Shader->getUniformHandles().begin(); it != Shader->getUniformHandles().end(); ++ it)
    {
        std::map<std::string, SUniform>::iterator jt = Uniforms.find(it->first);
        if (jt != Uniforms.end())
            jt->second.Handle = it->second.Handle;
        else if (it->first != "uModelMatrix" && it->first != "uProjMatrix" && it->first != "uViewMatrix")
            std::cout << "Uniform required by shader but not found in renderable: " << it->first << std::endl;
    }
}

CTexture * CRenderable::getTexture()
{
    return Texture;
}

void CRenderable::setTexture(CTexture * texture)
{
    Texture = texture;
}

GLenum const CRenderable::getDrawType() const
{
    return DrawType;
}

void CRenderable::setDrawType(GLenum const drawType)
{
    DrawType = drawType;
}

CBufferObject<GLushort> * CRenderable::getIndexBufferObject()
{
    return IndexBufferObject;
}

void CRenderable::setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject)
{
    IndexBufferObject = indexBufferObject;
}

void CRenderable::draw(CCamera const & Camera)
{
    CShader * CopyShader = Shader;

    if (isDebugDataEnabled(EDebugData::NormalColors))
    {
        if (! NormalColorShader)
            NormalColorShader = CShaderLoader::loadShader("Shaders/normalColor");
        setShader(NormalColorShader);
    }

    if (! Shader || ! IndexBufferObject)
        return;

    if (! IndexBufferObject->isIndexBuffer())
        return;

    CShaderContext ShaderContext(* Shader);
    for (std::map<std::string, SAttribute>::iterator it = Attributes.begin(); it != Attributes.end(); ++ it)
    {
        if (it->second.Handle >= 0)
            it->second.Value->bindTo(it->second.Handle, ShaderContext);
    }

    for (std::map<std::string, SUniform>::iterator it = Uniforms.begin(); it != Uniforms.end(); ++ it)
    {
        if (it->second.Handle >= 0)
            it->second.Value->bindTo(it->second.Handle, ShaderContext);
    }

    for (std::set<GLenum const>::iterator it = RenderModes.begin(); it != RenderModes.end(); ++ it)
        glEnable(* it);

    if (Texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture->getTextureHandle());
    }

    glm::mat4 Transformation = glm::translate(glm::mat4(1.0f), Translation.getGLMVector());
    Transformation = glm::rotate(Transformation, Rotation.X, glm::vec3(1, 0, 0));
    Transformation = glm::rotate(Transformation, Rotation.Y, glm::vec3(0, 1, 0));
    Transformation = glm::rotate(Transformation, Rotation.Z, glm::vec3(0, 0, 1));
    Transformation = glm::scale(Transformation, Scale.getGLMVector());

    ShaderContext.uniform("uModelMatrix", Transformation);
    ShaderContext.uniform("uViewMatrix", Camera.getViewMatrix());
    ShaderContext.uniform("uProjMatrix", Camera.getProjectionMatrix());

    if (IndexBufferObject->isDirty())
        IndexBufferObject->syncData();

    ShaderContext.bindIndexBufferObject(IndexBufferObject->getHandle());

    glDrawElements(DrawType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);

    if (isDebugDataEnabled(EDebugData::Normals) && NormalObject)
    {
        NormalObject->setTranslation(Translation);
        NormalObject->setScale(Scale);
        NormalObject->setRotation(Rotation);
        NormalObject->draw(Camera);
    }

    for (std::set<GLenum const>::iterator it = RenderModes.begin(); it != RenderModes.end(); ++ it)
        glDisable(* it);

    if (isDebugDataEnabled(EDebugData::NormalColors))
        setShader(CopyShader);
}

SBoundingBox3 const & CRenderable::getBoundingBox() const
{
    return BoundingBox;
}

SBoundingBox3 & CRenderable::getBoundingBox()
{
    return BoundingBox;
}

void CRenderable::addAttribute(std::string const & label, IAttribute * attribute)
{
    std::map<std::string, SAttribute>::iterator it = Attributes.find(label);

    if (it != Attributes.end())
    {
        delete it->second.Value;
    }

    Attributes[label] = SAttribute(attribute);
}

void CRenderable::addUniform(std::string const & label, IUniform * uniform)
{
    std::map<std::string, SUniform>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
    {
        delete it->second.Value;
    }

    Uniforms[label] = SUniform(uniform);
}

void CRenderable::removeAttribute(std::string const & label)
{
    std::map<std::string, SAttribute>::iterator it = Attributes.find(label);

    if (it != Attributes.end())
    {
        delete it->second.Value;
        Attributes.erase(it);
    }
}

void CRenderable::removeUniform(std::string const & label)
{
    std::map<std::string, SUniform>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
    {
        delete it->second.Value;
        Uniforms.erase(it);
    }
}

void CRenderable::addRenderMode(GLenum const mode)
{
    RenderModes.insert(mode);
}

void CRenderable::removeRenderMode(GLenum const mode)
{
    RenderModes.erase(mode);
}

bool const CRenderable::isDebugDataEnabled(EDebugData::Domain const type) const
{
    return (type & DebugDataFlags) != 0;
}

void CRenderable::enableDebugData(EDebugData::Domain const type)
{
    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags |= type;
}

void CRenderable::disableDebugData(EDebugData::Domain const type)
{
    if (type == EDebugData::None)
        DebugDataFlags = 0;
    else
        DebugDataFlags ^= type;
}
