#include "CRenderable.h"

#include <iostream>

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

#include "CShaderLoader.h"
#include "CSceneManager.h"


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

CMat4Uniform::CMat4Uniform()
{}

CMat4Uniform::CMat4Uniform(glm::mat4 const & value)
    : Value(value)
{}

void CMat4Uniform::bindTo(GLuint const uniformHandle, CShaderContext & shaderContext)
{
    shaderContext.uniform(uniformHandle, Value);
}


CRenderable::SAttribute::SAttribute()
    : Handle(-1)
{}

CRenderable::SAttribute::SAttribute(boost::shared_ptr<IAttribute> value)
    : Value(value), Handle(-1)
{}


CRenderable::SUniform::SUniform()
    : Handle(-1)
{}

CRenderable::SUniform::SUniform(boost::shared_ptr<IUniform> value)
    : Value(value), Handle(-1)
{}


SMaterial::SMaterial()
    : Shader(0), Texture(0)
{}


CRenderable::CRenderable()
    : DrawType(GL_TRIANGLES), NormalObject(0),  NormalColorShader(0), IndexBufferObject(0)
{
    uModelMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());
    uNormalMatrix = boost::shared_ptr<CMat4Uniform>(new CMat4Uniform());

    addUniform("uModelMatrix", uModelMatrix);
    addUniform("uNormalMatrix", uNormalMatrix);
}

SMaterial & CRenderable::getMaterial()
{
    return Material;
}

SMaterial const & CRenderable::getMaterial() const
{
    return Material;
}

void CRenderable::loadHandlesFromShader(CShader const * const shader, CScene const * const scene)
{
    if (LastLoadedShader == shader && LastLoadedScene == scene)
        return;

    // Remove any handles a previous shader might have set
    for (std::map<std::string, SAttribute>::iterator it = Attributes.begin(); it != Attributes.end(); ++ it)
        it->second.Handle = -1;
    for (std::map<std::string, SUniform>::iterator it = Uniforms.begin(); it != Uniforms.end(); ++ it)
        it->second.Handle = -1;
    SceneLoadedUniforms.clear();

    LastLoadedShader = shader;
    LastLoadedScene = scene;

    if (! LastLoadedShader)
        return;

    // Check the existence of all required shader attributes
    for (std::map<std::string, SShaderVariable>::const_iterator it = LastLoadedShader->getAttributeHandles().begin(); it != LastLoadedShader->getAttributeHandles().end(); ++ it)
    {
        std::map<std::string, SAttribute>::iterator jt;
        if ((jt = Attributes.find(it->first)) != Attributes.end())
            jt->second.Handle = it->second.Handle;
        else
            std::cout << "Attribute required by shader but not found in renderable: " << it->first << std::endl;
    }

    // Check the existences of all required uniform uniforms - skipping implicit uniforms
    for (std::map<std::string, SShaderVariable>::const_iterator it = LastLoadedShader->getUniformHandles().begin(); it != LastLoadedShader->getUniformHandles().end(); ++ it)
    {
        std::map<std::string, SUniform>::iterator jt;
        std::map<std::string, SUniform>::const_iterator kt;
        if ((jt = Uniforms.find(it->first)) != Uniforms.end())
            jt->second.Handle = it->second.Handle;
        else if ((kt = scene->getUniforms().find(it->first)) != scene->getUniforms().end())
        {
            SceneLoadedUniforms[kt->first] = kt->second;
            SceneLoadedUniforms[kt->first].Handle = it->second.Handle;
        }
        else
            std::cout << "Uniform required by shader but not found in renderable or scene: " << it->first << std::endl;
    }
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

void CRenderable::draw(CScene const * const scene)
{
    // If no ibo loaded, we can't draw anything
    // If the ibo loaded hasn't been synced as an index buffer object, 
    if (! IndexBufferObject || ! IndexBufferObject->isIndexBuffer() || ! Visible)
        return;

    CShader * ShaderToUse = Material.Shader;

    if (! ShaderToUse)
        ShaderToUse = CShaderLoader::loadShader("Simple");

    // If normal colors are being shown, switch to the normal color shader
    if (isDebugDataEnabled(EDebugData::NormalColors))
    {
        if (! NormalColorShader)
            NormalColorShader = CShaderLoader::loadShader("NormalColor");
        ShaderToUse = NormalColorShader;
    }

    loadHandlesFromShader(ShaderToUse, scene);

    // Create shader context and link all variables required by the shader
    CShaderContext ShaderContext(* ShaderToUse);

    // Set up transform matrices
    uModelMatrix->Value = glm::translate(glm::mat4(1.0f), Translation.getGLMVector());
    uModelMatrix->Value = glm::rotate(uModelMatrix->Value, Rotation.X, glm::vec3(1, 0, 0));
    uModelMatrix->Value = glm::rotate(uModelMatrix->Value, Rotation.Y, glm::vec3(0, 1, 0));
    uModelMatrix->Value = UsesRotationMatrix ? uModelMatrix->Value * RotationMatrix : glm::rotate(uModelMatrix->Value, Rotation.Z, glm::vec3(0, 0, 1));
    uModelMatrix->Value = glm::scale(uModelMatrix->Value, Scale.getGLMVector());

    // Pass transform matrices to shader
    uNormalMatrix->Value = glm::transpose(glm::inverse(uModelMatrix->Value));

    // Pass values to shader
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
    for (std::map<std::string, SUniform>::iterator it = SceneLoadedUniforms.begin(); it != SceneLoadedUniforms.end(); ++ it)
    {
        it->second.Value->bindTo(it->second.Handle, ShaderContext);
    }

    // Set up texturing if a texture was supplied
    if (Material.Texture)
    {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Material.Texture->getTextureHandle());
    }

    // If the ibo is dirty, sync it!
    if (IndexBufferObject->isDirty())
        IndexBufferObject->syncData();

    // And bind the synced buffer object to shader...
    ShaderContext.bindIndexBufferObject(IndexBufferObject->getHandle());

    // Finally draw!
    glDrawElements(DrawType, IndexBufferObject->getElements().size(), GL_UNSIGNED_SHORT, 0);

    // Draw the normal object if it is enabled
    if (isDebugDataEnabled(EDebugData::Normals) && NormalObject)
    {
        NormalObject->setTranslation(Translation);
        NormalObject->setScale(Scale);
        NormalObject->setRotation(Rotation);
        NormalObject->draw(scene);
    }

    // Cleanup the texture if it was used
    if (Material.Texture)
    {
        glDisable(GL_TEXTURE_2D);
    }
}

void CRenderable::addAttribute(std::string const & label, boost::shared_ptr<IAttribute> attribute)
{
    Attributes[label] = SAttribute(attribute);
}

void CRenderable::addUniform(std::string const & label, boost::shared_ptr<IUniform> uniform)
{
    Uniforms[label] = SUniform(uniform);
}

void CRenderable::removeAttribute(std::string const & label)
{
    std::map<std::string, SAttribute>::iterator it = Attributes.find(label);

    if (it != Attributes.end())
        Attributes.erase(it);
}

void CRenderable::removeUniform(std::string const & label)
{
    std::map<std::string, SUniform>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        Uniforms.erase(it);
}
