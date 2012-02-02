#include "CRenderable.h"


CRenderable::CRenderable()
    : Scale(1)
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

void CRenderable::draw(CCamera const & Camera)
{
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
    }

    Attributes.erase(it);
}

void CRenderable::removeUniform(std::string const & label)
{
    std::map<std::string, SUniform>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
    {
        delete it->second.Value;
    }

    Uniforms.erase(it);
}

void CRenderable::addRenderMode(GLenum const mode)
{
    RenderModes.insert(mode);
}

void CRenderable::removeRenderMode(GLenum const mode)
{
    RenderModes.erase(mode);
}
