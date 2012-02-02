#include "IRenderable.h"


IRenderable::IRenderable()
    : Scale(1)
{}


SVector3 const & IRenderable::getTranslation() const
{
    return Translation;
}

SVector3 const & IRenderable::getRotation() const
{
    return Rotation;
}

SVector3 const & IRenderable::getScale() const
{
    return Scale;
}


void IRenderable::setTranslation(SVector3 const & translation)
{
    Translation = translation;
}

void IRenderable::setRotation(SVector3 const & rotation)
{
    Rotation = rotation;
}

void IRenderable::setScale(SVector3 const & scale)
{
    Scale = scale;
}

void IRenderable::draw(CCamera const & Camera)
{
    //CShaderContext ShaderContext(* Shader);
    //for (std::map<std::string, IAttribute *>::iterator it = Attributes.begin(); 
}

SBoundingBox3 const & IRenderable::getBoundingBox() const
{
    return BoundingBox;
}

SBoundingBox3 & IRenderable::getBoundingBox()
{
    return BoundingBox;
}

void IRenderable::addAttribute(std::string const & label, IAttribute * attribute)
{
    std::map<std::string, IAttribute *>::iterator it = Attributes.find(label);

    if (it != Attributes.end())
    {
        delete it->second;
    }

    Attributes[label] = attribute;
}

void IRenderable::addUniform(std::string const & label, IUniform * uniform)
{
    std::map<std::string, IUniform *>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
    {
        delete it->second;
    }

    Uniforms[label] = uniform;
}
