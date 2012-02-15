#ifndef _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_
#define _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_

struct SMaterial
{
    CShader * Shader;
    CTexture * Texture;

    boost::shared_ptr<CVec3Uniform> AmbientColor;
    boost::shared_ptr<CVec3Uniform> DiffuseColor;
    boost::shared_ptr<CFloatUniform> Shininess;

    SMaterial();
};

#endif
