#include "CRenderable.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"


CRenderable::CRenderable()
    : Scale(1), Shader(0), Texture(0)
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
        else
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

    glDrawElements(GL_TRIANGLES, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);


    //Attributes["aNormalLine"].Value->bindTo(Attributes["aPosition"].Handle, ShaderContext);
    //glDrawArrays(GL_LINES, 0, 1298);

    for (std::set<GLenum const>::iterator it = RenderModes.begin(); it != RenderModes.end(); ++ it)
        glDisable(* it);
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
