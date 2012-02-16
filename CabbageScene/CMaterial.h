#ifndef _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_
#define _CABBAGE_SCENE_CMATERIAL_H_INCLUDED_

#include "SUniform.h"
#include "SAttribute.h"
#include "CTexture.h"

class CMaterial
{

	SUniform<SColor> BindAmbientColor, BindDiffuseColor;
	SUniform<float> BindShininess;

public:

    CTexture * Texture;
	SColor AmbientColor, DiffuseColor;
	float Shininess;

    CMaterial();

	void loadDefaults();

	IUniform const * const getUniform(std::string const & label);

};

#endif
